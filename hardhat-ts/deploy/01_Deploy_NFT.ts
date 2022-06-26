import { network } from "hardhat"
import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
const { ethers } = require("hardhat")
const func: DeployFunction = async function (hre: HardhatRuntimeEnvironment) {
    const { deployments, getNamedAccounts, network } = hre as any
    const { deploy } = deployments
    const { deployer } = await getNamedAccounts()

    // const DustBoyNFTContract = await ethers.getContractFactory("DustBoyNFT");
    // const DustBoyNFT = await DustBoyNFTContract.deploy();
    // await DustBoyNFT.deployed();

    const NAME = 'DustBoyNFT'
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
func.tags = ["DustBoyNFT"]

