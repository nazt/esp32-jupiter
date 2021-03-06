// SPDX-License-Identifier: MIT

pragma solidity ^0.8.9;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "@openzeppelin/contracts/token/ERC721/extensions/ERC721Enumerable.sol";
import "@openzeppelin/contracts/interfaces/IERC2981.sol";
import "@openzeppelin/contracts/utils/Counters.sol";
import "@openzeppelin/contracts/utils/Address.sol";

contract OracleNFT is ERC721Enumerable, IERC2981, ReentrancyGuard, Ownable {
    using Counters for Counters.Counter;

    struct Sensor {
        uint256 pm1;
        uint256 pm10;
        uint256 pm2_5;
        uint256 temp;
        uint256 humid;
        uint256 timestamp;
    }

    mapping(uint256 => Sensor) public sensors;

    event EnvironmentalChanged(uint256 id, uint256 pm1, uint256 pm10, uint256 pm2_5, uint256 temp, uint256 humid, uint256 timestamp);


    constructor() ERC721("OracleNFT", "ORC") {
        customBaseURI = "";
    }

    function setEnvironment(uint256 _tokenId, uint256 _pm1, uint256 _pm10, uint256 _pm2_5, uint256 _temp, uint256 _humid, uint256 _refTimestamp) public {
        if (_refTimestamp == 0) {
            _refTimestamp = block.timestamp;
        }
        sensors[_tokenId].pm1 = _pm1;
        sensors[_tokenId].pm10 = _pm10;
        sensors[_tokenId].pm2_5 = _pm2_5;
        sensors[_tokenId].temp = _temp;
        sensors[_tokenId].humid = _humid;
        sensors[_tokenId].timestamp = _refTimestamp;
        emit EnvironmentalChanged(_tokenId, _pm1, _pm10, _pm2_5, _temp, _humid, _refTimestamp);
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
