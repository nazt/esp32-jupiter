// SPDX-License-Identifier: MIT

pragma solidity ^0.8.9;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "@openzeppelin/contracts/token/ERC721/extensions/ERC721Enumerable.sol";
import "@openzeppelin/contracts/interfaces/IERC2981.sol";
import "@openzeppelin/contracts/utils/Counters.sol";
import "@openzeppelin/contracts/utils/Address.sol";

contract DustBoyNFT is ERC721Enumerable, IERC2981, ReentrancyGuard, Ownable {
    using Counters for Counters.Counter;

    // struct Sensor {
    //     mapping(address => bool) claimed;
    //     Counters.Counter supply;
    // }
    struct Sensor {
        uint256 field1;
        uint256 field2;
        uint256 field3;
        uint256 field4;
    }

    mapping(uint256 => Sensor) public sensors;

    constructor() ERC721("DustBoyNFT", "DBNFT") {
        customBaseURI = "https://dustboy-metadata.laris.workers.dev/";
    }

    function setSensor(uint256 _tokenId, Sensor calldata s) public {
        require(ownerOf(_tokenId) == msg.sender);
        sensors[_tokenId].field1 = s.field1;
        sensors[_tokenId].field2 = s.field2;
        sensors[_tokenId].field2 = s.field3;
    }

    /** MINTING **/

    uint256 public constant MAX_SUPPLY = 20000;

    function mint(uint256 id) public payable nonReentrant {
        require(saleIsActive, "Sale not active");

        require(totalSupply() < MAX_SUPPLY, "Exceeds max supply");

        _mint(msg.sender, id);
    }

    /** ACTIVATION **/

    bool public saleIsActive = true;

    function setSaleIsActive(bool saleIsActive_) external onlyOwner {
        saleIsActive = saleIsActive_;
    }

    /** URI HANDLING **/

    string private customBaseURI;

    function setBaseURI(string memory customBaseURI_) external onlyOwner {
        customBaseURI = customBaseURI_;
    }

    function _baseURI() internal view virtual override returns (string memory) {
        return customBaseURI;
    }

    /** PAYOUT **/

    function withdraw() public nonReentrant {
        uint256 balance = address(this).balance;

        Address.sendValue(payable(owner()), balance);
    }

    function withdraw(uint256 value) public onlyOwner {
        uint256 balance = address(this).balance;
        require(balance > 0, "No ether left to withdraw");
        payable(owner()).transfer(value);
    }

    /** ROYALTIES **/

    function royaltyInfo(uint256, uint256 salePrice)
        external
        view
        override
        returns (address receiver, uint256 royaltyAmount)
    {
        return (address(this), (salePrice * 1000) / 10000);
    }

    function supportsInterface(bytes4 interfaceId)
        public
        view
        virtual
        override(ERC721Enumerable, IERC165)
        returns (bool)
    {
        return (interfaceId == type(IERC2981).interfaceId || super.supportsInterface(interfaceId));
    }
}
