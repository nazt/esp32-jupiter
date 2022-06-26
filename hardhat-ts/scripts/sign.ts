import { Signer } from "ethers"
import { ethers } from "hardhat"
const Web3 = require("web3")

const web3 = new Web3("https://babel-api.mainnet.iotex.io")
const fetch = require("node-fetch")
const hre = require("hardhat")
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners()

    // const txParams = {
    //     nonce: ethers.utils.hexlify(973),
    //     gasPrice: "0x09184e72a000",
    //     gasLimit: ethers.utils.hexlify(2100),
    //     to: "0x0000000000000000000000000000000000000000",
    //     value: "0x00",
    //     data: "0x7f7465737432000000000000000000000000000000000000000000000000000000600057",
    // }

    // const common = new Common({ chain: Chain.Rinkeby })
    // const tx = Transaction.fromTxData(txParams, { common })

    // const privateKey = Buffer.from(
    //     "",
    //     "hex",
    // )

    // const signedTx = tx.sign(privateKey)

    // const serializedTx = signedTx.serialize()
    // console.log("0x" + serializedTx.toString("hex"))

    // console.log(signedTx)

    // let transaction = {
    //     ...txParams,
    //     nonce: ethers.utils.hexlify(973),
    //     maxPriorityFeePerGas: ethers.utils.parseUnits("5", "gwei"),
    //     maxFeePerGas: ethers.utils.parseUnits("20", "gwei"),
    //     chainId: 4,
    //     type: 2,
    // }

    // console.log(accounts[0])
    let privatekey = ""
    let wallet = new ethers.Wallet(privatekey)
    console.log(
        "xxx",
        await hre.ethers.provider.getTransactionCount(wallet.address),
        wallet.address,
    )
    const newNonce = await hre.ethers.provider.getTransactionCount(wallet.address)
    console.log("newNonce", newNonce)
    // let transaction = {
    //     to: "0x556612d16993ed197e04bfccde1f665d1b788baf",
    //     gasLimit: 81000,
    //     from: "0x54123b2309D456261410e51C10732033Ec885c21",
    //     nonce: newNonce,
    //     gasPrice: hre.ethers.utils.hexlify(20000000000),
    //     // type: 2,
    //     chainId: 4689,
    //     data: "0xc47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000036161610000000000000000000000000000000000000000000000000000000000",
    //     // data: "0x68656c6c6f20776f726c6421",
    // }
    // const newNonce = await web3.eth
    //     .getTransactionCount("0x54123b2309D456261410e51C10732033Ec885c21")
    //     .then(console.log)
    let transaction = {
        to: "0x556612d16993ed197e04bfccde1f665d1b788baf",
        // gasLimit: 81000,
        from: "0x54123b2309D456261410e51C10732033Ec885c21",
        nonce: newNonce,
        type: 0,
        chainId: 4689,
        gasPrice: "0xe8d4a51000",
        gasLimit: "0x013c68",
        data: "0xc47f0027000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000036161610000000000000000000000000000000000000000000000000000000000",
        // data: "0x68656c6c6f20776f726c6421",
    }
    let rawTransaction = await wallet.signTransaction(transaction)
    console.log("sending")
    // const out = await hre.ethers.provider.sendTransaction(transaction)
    // console.log(out)
    console.log(rawTransaction)
    const signedTx = await web3.eth.accounts.signTransaction(
        transaction,
        "",
        // "",
    )
    // const out = await hre.ethers.provider.sendTransaction(rawTransaction)
    // const out = await web3.eth.sendSignedTransaction(rawTransaction).then(console.log)
    // const out = await web3.eth.sendSignedTransaction(signedTx.rawTransaction).then(console.log)
    // console.log("out", out)
    // console.log(rawTransaction)
    // console.log(ethers.utils.RLP.encode(transaction))
    // console.log(transaction.serialize())
    // console.log(hre.ethers.utils.serializeTransaction(transaction))
    // const out = await hre.ethers.provider.sendTransaction(transaction)
    // console.log(out)
    // const out = await hre.ethers.provider.sendTransaction(ethers.utils.RLP.encode(rawTransaction))
    // console.log(out)
    // console.log(rawTransaction)
    // const out = await hre.ethers.provider.sendTransaction(rawTransaction)
    // console.log(out)
    // await wallet.sendTransaction(rawTransaction)

    //   let rawTransaction = await accounts[0].signTransaction(transaction).then(e => {
    //     console.log(e)
    //   });

    // .then(ethers.utils.serializeTransaction(transaction))

    // let rawTransaction = await wallet.signTransaction(transaction)
    // console.log(rawTransaction)
    //   .then(ethers.utils.serializeTransaction(transaction));
    //   console.log('Raw txhash string ' + rawTransaction);
    //      let x = await wallet.signTransaction(transaction)

    //      console.log('x', x)
    //     //  .then(ethers.utils.serializeTransaction(transaction));
    //     console.log(ethers.utils.serializeTransaction(transaction));

    // print the raw transaction hash
    //   console.log('Raw txhash string ' + rawTransaction);
}

if (require.main === module) {
    main()
        .then(() => process.exit(0))
        .catch(error => {
            console.error(error)
            process.exit(1)
        })
} else {
    module.exports = main
}
