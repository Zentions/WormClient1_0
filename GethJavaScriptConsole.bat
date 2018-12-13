cd /
E:
cd ethereum\Geth
geth --identity "PICCetherum" --datadir "./chain" --nodiscover --rpc --rpccorsdomain "*" --port "30303" --rpcapi "db,eth,net,web3"  --networkid 1500 console 2>>geth.log