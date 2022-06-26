pragma solidity 0.8.9;

import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/utils/math/SafeMath.sol";
import "@openzeppelin/contracts/utils/Strings.sol";

import "./DustBoyNFT.sol";
import "./Buffer.sol";


contract Redemption is Ownable {
    DustBoyNFT public nft;

    mapping(uint256 => string) public uriMap;
    string baseURI = "https://drive.google.com/drive/folders/";
    string suffix = ".mp4";

    constructor() {
        nft = DustBoyNFT(0x63e74C374c02B6c12467f56747E4aB9904C1a364);
    }

    function Existing(address _t) public {
        nft = DustBoyNFT(_t);
    }

    function getTokenUriFromAddress(uint32 _tokenId) public view returns (string memory) {
        return nft.tokenURI(_tokenId);
    }

    function setUri(uint32 _tokenId, string memory _uri) public {
        uriMap[_tokenId] = _uri;
    }

    function getItems(address _owner) public view returns (uint256[] memory) {
        uint256 balance = nft.balanceOf(_owner);
        uint256[] memory items = new uint256[](balance);
        uint256 i = 0;
        while (i < nft.balanceOf(_owner)) {
            items[i] = nft.tokenOfOwnerByIndex(_owner, i);
            i++;
        }
        return items;
    }

    function getUrl(address _owner) public view returns (string memory) {
        // uint256 balance = nft.balanceOf(_owner);
        // string[] memory items;
        bytes memory result = new bytes(4096);
        Buffer.append(result, "");
        Buffer.append(result, baseURI);
        Buffer.append(result, Strings.toString(nft.tokenOfOwnerByIndex(_owner, 0)));
        Buffer.append(result, suffix);
        uint256 i = 1;
        while (i < nft.balanceOf(_owner)) {
            Buffer.append(result, ",");
            Buffer.append(result, baseURI);
            Buffer.append(result, Strings.toString(nft.tokenOfOwnerByIndex(_owner, i)));
            Buffer.append(result, suffix);
            // concat string to items string[]
            // String.toString(nft.tokenOfOwnerByIndex(_owner, i));
            i++;
        }
        return Buffer.toString(result);
    }
}
