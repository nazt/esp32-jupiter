import { time } from "console"
import { before, beforeEach } from "mocha"
import { deployContract, fastForwardTimestamp, fastForwardTimestampInDays } from "./utils"

const { expect } = require("chai")
const { ethers } = require("hardhat")
const { BigNumber, providers } = require("ethers")
const { MerkleTree } = require("merkletreejs")
const keccak256 = require("keccak256")

describe("# Minting Test", function () {
    const whitelistPrice = ethers.utils.parseEther("0.144")

})

