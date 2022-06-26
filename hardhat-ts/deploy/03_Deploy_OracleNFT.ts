import { ethers, network } from "hardhat"
import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
import { from } from "@iotexproject/iotex-address-ts";

const TAG = 'OracleNFT'
const func: DeployFunction = async function (hre: HardhatRuntimeEnvironment) {
    const { deployments, getNamedAccounts, network } = hre
    const { deploy, execute } = deployments
    const { deployer } = await getNamedAccounts()

    const args: any[] = []
    const result = await deploy(TAG, {
        contract: TAG,
        from: deployer,
        args,
        log: true,
    })

    const receipt = await execute("OracleNFT", {from: deployer}, "mint", [4])
    console.log('minted at', receipt.transactionHash)

    console.log(
        `npx hardhat verify ${result.address} ${args
            .map(arg => `"${arg}"`)
            .join(" ")} --network ${network.name}`,
    )

    console.log(`ethAddress: ${from(result.address).stringEth()}`)
    console.log(`iotexAddress: ${from(result.address).string()}`)

}

export default func
func.tags = [TAG]

