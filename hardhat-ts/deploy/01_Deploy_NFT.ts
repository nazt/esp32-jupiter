import { ethers, network } from "hardhat"
import { DeployFunction } from "hardhat-deploy/types"
import { HardhatRuntimeEnvironment } from "hardhat/types"
import { from } from "@iotexproject/iotex-address-ts";
const TAG = 'DustBoyNFT'
const func: DeployFunction = async function (hre: HardhatRuntimeEnvironment) {
    const { deployments, getNamedAccounts, network } = hre
    const { deploy } = deployments
    const { deployer } = await getNamedAccounts()

    // const DustBoyNFTContract = await ethers.getContractFactory("DustBoyNFT");
    // const DustBoyNFT = await DustBoyNFTContract.deploy();
    // await DustBoyNFT.deployed();
    // const deployedContract = await ethers.getContract(NAME)

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

    console.log(`iotexAddress: ${from(result.address)}`)

}

export default func
func.tags = [TAG]

