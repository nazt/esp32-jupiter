library Buffer {
    function hasCapacityFor(bytes memory buffer, uint256 needed) internal pure returns (bool) {
        uint256 size;
        uint256 used;
        assembly {
            size := mload(buffer)
            used := mload(add(buffer, 32))
        }
        return size >= 32 && used <= size - 32 && used + needed <= size - 32;
    }

    function toString(bytes memory buffer) internal pure returns (string memory) {
        require(hasCapacityFor(buffer, 0), "Buffer.toString: invalid buffer");
        string memory ret;
        assembly {
            ret := add(buffer, 32)
        }
        return ret;
    }

    function append(bytes memory buffer, string memory str) internal view {
        require(hasCapacityFor(buffer, bytes(str).length), "Buffer.append: no capacity");
        assembly {
            let len := mload(add(buffer, 32))
            pop(
                staticcall(
                    gas(),
                    0x4,
                    add(str, 32),
                    mload(str),
                    add(len, add(buffer, 64)),
                    mload(str)
                )
            )
            mstore(add(buffer, 32), add(len, mload(str)))
        }
    }
}
