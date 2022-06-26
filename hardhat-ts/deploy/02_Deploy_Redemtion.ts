import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
// const { ethers } = require("hardhat")
import { ethers, network } from "hardhat"
const func: DeployFunction = async function (hre: HardhatRuntimeEnvironment) {
    const { deployments, getNamedAccounts, network } = hre
    const { deploy } = deployments
    const { deployer } = await getNamedAccounts()

    const NAME = 'Redemption'
    const args: any[] = []
    await deploy(NAME, {
        contract: NAME,
        from: deployer,
        args,
        log: true,
    })


    const deployedContract = await ethers.getContract(NAME)

    console.log(
        `npx hardhat verify ${deployedContract.address} ${args
            .map(arg => `"${arg}"`)
            .join(" ")} --network ${network.name}`,
    )

}
export default func
func.tags = ["Redemption"]

