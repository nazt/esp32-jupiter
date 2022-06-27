import { ethers, network } from "hardhat"
import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
import { from } from "@iotexproject/iotex-address-ts";

const TAG = 'JupiterOracle'
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

    console.log(`ethAddress: ${from(result.address).stringEth()}`)
    console.log(`iotexAddress: ${from(result.address).string()}`)

    const receipt = await execute("JupiterOracle", {from: deployer}, "mint", [1])
    console.log('minted at', receipt.transactionHash)

    console.log(
        `npx hardhat verify ${result.address} ${args
            .map(arg => `"${arg}"`)
            .join(" ")} --network ${network.name}`,
    )


}

export default func
func.tags = [TAG]

