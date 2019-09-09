#!/bin/sh

worbli system newaccount worbli.admin --transfer regulator EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 300
worbli set contract regulator /home/rob/git/compliance-contracts-cmake/build/contracts/worbli.compliance -p regulator


worbli system newaccount worbli.admin --transfer provider1 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider1 /home/rob/git/compliance-contracts-cmake/build/contracts/provider -p provider1

worbli system newaccount worbli.admin --transfer provider2 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider1 /home/rob/git/compliance-contracts-cmake/build/contracts/provider -p provider1

worbli system newaccount worbli.admin --transfer client EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract client /home/rob/git/compliance-contracts-cmake/build/contracts/client -p client

worbli system newaccount worbli.admin --transfer alice EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200



worbli push action regulator addcred '["kyc", "kyc verification"]' -p regulator
worbli push action regulator addcred '["accredited", "accredited investor verification"]' -p regulator
worbli push action regulator addprovider '["provider1", "Indentity provider"]' -p regulator
worbli push action regulator addprovcred '["provider1", "kyc"]' -p regulator
worbli push action regulator addprovcred '["provider1", "accredited"]' -p regulator


worbli push action provider1 addentry '["alice", "kyc", "true"]' -p provider1
worbli push action provider1 addentry '["alice", "accredited", "true"]' -p provider1

worbli push action client booltest '["alice"]' -p client



