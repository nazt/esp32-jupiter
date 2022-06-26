// const { ethers } = require("hardhat")
import { ethers } from "ethers"

const data = Buffer.from(
    "xH8AJwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAANhYWEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==",
    "base64",
).toString("hex")

const sig = Buffer.from(
    "A2RyY56V9MR7GtRGDB2+JiA2K+0NYCYuunB/r7jo3ZBzJekdWJ2iBofQRPOcVxmi8IF7X9RUWTDFGYQvtAKXqwE=",
    "base64",
).toString("hex")

console.log(data, sig)
const out = ethers.utils.verifyMessage(data, "0x" + sig)
console.log(out)
