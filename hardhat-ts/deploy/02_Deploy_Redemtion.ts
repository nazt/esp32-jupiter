import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
import { ethers, network } from "hardhat"

const TAG = 'Redemption'
const func: DeployFunction = async function (hre: HardhatRuntimeEnvironment) {
    const { deployments, getNamedAccounts, network } = hre
    const { deploy } = deployments
    const { deployer } = await getNamedAccounts()

    const args: any[] = []
    const result = await deploy(TAG, {
        contract: TAG,
        from: deployer,
        args,
        log: true,
    })


    console.log(
        `npx hardhat verify ${result.address} ${args
            .map(arg => `"${arg}"`)
            .join(" ")} --network ${network.name}`,
    )

}
export default func
func.tags = [TAG]

