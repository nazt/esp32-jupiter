const Web3 = require("web3")

// const web3 = new Web3("https://babel-api.mainnet.iotex.io")
const web3 = new Web3("https://babel-api.testnet.iotex.io")

// const transaction = {

async function run() {
    const newNonce = await web3.eth
        .getTransactionCount("0x54123b2309D456261410e51C10732033Ec885c21")
        .then(console.log)
    let transaction = {
        to: "0xf98c0dd5d0ce940718037ca0a3ac61cebd447984",
        // value: ethers.utils.parseEther("0.1"),
        gasLimit: 81000,
        // maxPriorityFeePerGas: ethers.utils.parseUnits("5", "gwei"),
        // maxFeePerGas: ethers.utils.parseUnits("20", "gwei"),
        from: "0x54123b2309D456261410e51C10732033Ec885c21",
        nonce: newNonce,
        type: 0,
        chainId: 4690,
        // chainId: ethers.utils.hexlify(4689),
        data: "0xa4136862000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000036161610000000000000000000000000000000000000000000000000000000000",
        // data: "0x68656c6c6f20776f726c6421",
    }
    const signedTx = await web3.eth.accounts.signTransaction(
        transaction,
        "",
        // "",
    )
    console.log(signedTx.rawTransaction)
    await web3.eth.sendSignedTransaction(signedTx.rawTransaction).then(console.log)
    console.log(signedTx)
}

run()
