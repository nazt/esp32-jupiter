import { Signer } from "ethers"

const fetch = require("node-fetch")
const hre = require("hardhat")
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners()

    const tx = await hre.ethers.provider.getTransaction(
        "0x5c82750685ab1ddadc74775fbfcb5d274ca1b1f15c681b1bd136feba6313027a",
    )
    const txReceipt = await hre.ethers.provider.getTransactionReceipt(
        "0x5c82750685ab1ddadc74775fbfcb5d274ca1b1f15c681b1bd136feba6313027a",
    )
    console.log(tx)
    console.log(txReceipt)
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
