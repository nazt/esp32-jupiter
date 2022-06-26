# https://github.com/trustwallet/assets/blob/master/Makefile

ifndef network
override network = localhost
endif


.PHONY: setup test action deploy mue typechain whitelist

setup:
	hh run scripts/00-setup.js  --network $(network)

deploy:
	hh deploy --network $(network) --tags $(tags)
	make typechain
	yarn webconfig:$(network)
	make setup

mue:
	hh run scripts/setup/06-mue.ts --network $(network)

test:
	hh test

whitelist:
	node scripts/whitelist-sync.js
	cat generated/whitelist.json
	# npm run whitelist
	cp -Rv generated/whitelist.json ../nft-web/src/lib/whitelist.json

node:
	hh node --no-deploy

fresh:
	hh clean && hh compile
	yarn build


typechain:
	-rm -R typechain
	-rm -R ../nft-web/src/generated/typechain
	hh typechain
	cp -R typechain ../nft-web/src/generated

# abi:
# 	cat deployments/${network}/MyNFT.json | jq .abi | jq
# 	@cat deployments/${network}/MyNFT.json | jq .address
# 	@cat deployments/${network}/MyNFT.json | jq .transactionHash

action:
	# echo argument is $(foo)
	# echo argument is $(argument)
	@echo "action! $(foo)"
