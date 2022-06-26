import "@nomiclabs/hardhat-ethers"
import "@nomiclabs/hardhat-etherscan"
import "@nomiclabs/hardhat-solhint"
import "@nomiclabs/hardhat-waffle"
import "@openzeppelin/hardhat-upgrades"
import "@tenderly/hardhat-tenderly"
import "@typechain/hardhat"
import "dotenv/config"
import "hardhat-abi-exporter"
import "hardhat-deploy"
import "hardhat-gas-reporter"
import "hardhat-watcher"
import { HardhatUserConfig, task } from "hardhat/config"
import { HttpNetworkUserConfig } from "hardhat/types"
import "solidity-coverage"

const fs = require("fs")

const { Confirm } = require("enquirer")

const MNEMONIC_MODE = true
const PRIVATE_KEY = process.env.PRIVATE_KEY || ""

const TARGET_NETWORK = process.env.NETWORK || "iotexTestnet"

const mnemonicPath = "./generated/mnemonic.secret"

function getMnemonic() {
    try {
        return fs.readFileSync(mnemonicPath).toString().trim()
    } catch (e) {
        // @ts-ignore
        if (TARGET_NETWORK !== "localhost") {
            console.log(
                "☢️ WARNING: No mnemonic file created for a deploy account. Try `yarn run generate` and then `yarn run account`.",
            )
        }
    }
    return ""
}


const ACCOUNTS = MNEMONIC_MODE ? { mnemonic: getMnemonic() } : [PRIVATE_KEY]

const config: HardhatUserConfig = {
    namedAccounts: {
        deployer: {
            default: 0, // here this will by default take the first account as deployer
        },
    },
    networks: {
        localhost: {
            url: "http://localhost:8545",
        },
        rinkeby: {
            url: "https://rinkeby.infura.io/v3/0b7da908f9f9495eb0f8de905c4fca66",
            accounts: ACCOUNTS,
        },
        iotexMainnet: {
            url: "https://babel-api.mainnet.iotex.io",
            accounts: ACCOUNTS,
            saveDeployments: true,
            timeout: 3 * 20000,
            chainId: 4689,
            gas: 8500000,
            gasPrice: 1000000000000,
        },
        iotexTestnet: {
            url: "https://babel-api.testnet.iotex.io",
            accounts: ACCOUNTS,
            chainId: 4690,
            gas: 8500000,
            saveDeployments: true,
            gasPrice: 1000000000000,
        },
    },

    etherscan: {
        apiKey: {
            rinkeby: process.env.ETHERSCAN_API_KEY,
            goerli: process.env.ETHERSCAN_API_KEY,
            mainnet: process.env.ETHERSCAN_API_KEY,
        },
    },
    solidity: {
        settings: {
            outputSelection: {
                "*": {
                    "*": [
                        "abi",
                        "evm.bytecode",
                        "evm.deployedBytecode",
                        "metadata", // <-- add this
                    ],
                },
            },
        },
        compilers: [
            {
                version: "0.8.9",
                settings: {
                    optimizer: {
                        enabled: true,
                        runs: 200,
                    },
                },
            },
        ],
    },
    paths: {
        cache: "./generated/cache",
        artifacts: "./generated/artifacts",
        deployments: "./generated/deployments",
    },
    typechain: {
        outDir: "./typechain",
    },
    watcher: {
        compile: {
            tasks: ["clean", "compile"],
            files: ["./contracts", "./test"],
            verbose: true,
        },
        ci: {
            tasks: [
                "clean",
                { command: "compile", params: { quiet: true } },
                {
                    command: "test",
                    params: { noCompile: true, testFiles: ["test/css.mint.test.ts"] },
                },
            ],
        },
    },
}
export default config

task("deploy")
    //   .addOptionalParam("network", "Network")
    .setAction(async (taskArgs, hre, runSuper) => {
        const { network } = taskArgs
        console.log(`Deploying...`)
        const accounts = await hre.ethers.getSigners()
        // console.log(accounts)

        const prompt = new Confirm({
            name: "question",
            message: `Confirm to deploy with ${accounts[0].address}`,
        })
            ; (await prompt.run()) && (await runSuper(taskArgs))
    })

const Table = require("cli-table3")
const hdkey = require("ethereumjs-wallet/hdkey")
const bip39 = require("bip39")
const EthUtil = require("ethereumjs-util")
const glob = require("glob")

task("generate", "Create a mnemonic for builder deploys", async (_, { ethers }) => {
    const mnemonic = bip39.generateMnemonic()
    const seed = await bip39.mnemonicToSeed(mnemonic)
    const hdwallet = hdkey.fromMasterSeed(seed)

    const walletHdpath = "m/44'/60'/0'/0/"
    const account_index = 0

    const fullPath = `${walletHdpath}${account_index}`
    const wallet = hdwallet.derivePath(fullPath).getWallet()
    const privateKey = `0x${wallet._privKey.toString("hex")}`
    const address = `0x${EthUtil.privateToAddress(wallet._privKey).toString("hex")}`
    console.log(`🔐 Account Generated as ${address} and set as mnemonic in packages/hardhat`)
    console.log("💬 Use 'yarn run account' to get more information about the deployment account.")

    const table = new Table({
        head: ["fullPath", "address", "privateKey", "mnemonic"],
    })

    table.push([fullPath, address, privateKey, mnemonic])
    fs.writeFileSync(mnemonicPath, mnemonic.toString())

    const files = glob.sync("./generated/*.secret")
    fs.writeFileSync(`./generated/${files.length}_${address}.secret`, mnemonic.toString())

    console.log(table.toString())
})

task("balance", "Get balance informations for the deployment account.", async (_, { ethers }) => {
    const mnemonic = fs.readFileSync(mnemonicPath).toString().trim()
    const seed = await bip39.mnemonicToSeed(mnemonic)
    const hdwallet = hdkey.fromMasterSeed(seed)
    const wallet_hdpath = "m/44'/60'/0'/0/"
    const account_index = 0
    const fullPath = wallet_hdpath + account_index
    const wallet = hdwallet.derivePath(fullPath).getWallet()
    const privateKey = `0x${wallet._privKey.toString("hex")}`

    const address = `0x${EthUtil.privateToAddress(wallet._privKey).toString("hex")}`

    const qrcode = require("qrcode-terminal")
    qrcode.generate(address)

    const table = new Table({
        head: ["fullPath", "address", "privateKey", "mnemonic"],
    })

    table.push([fullPath, address, "-", "-"])
    console.log(table.toString())
    fs.writeFileSync(mnemonicPath, mnemonic.toString())

    for (const n in config.networks) {
        try {
            const { url } = config.networks[n] as HttpNetworkUserConfig
            const provider = new ethers.providers.JsonRpcProvider(url)
            const balance = await provider.getBalance(address)
            console.log(` -- ${n} --  -- -- 📡 `)
            console.log(`   balance: ${ethers.utils.formatEther(balance)}`)
            console.log(`   nonce: ${await provider.getTransactionCount(address)}`)
        } catch (e) {
            // console.log("ERROR");
        }
    }
})
