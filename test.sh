#!/bin/sh

worbli system newaccount worbli.admin --transfer worbli.reg EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract worbli.reg /home/rob/git/compliance-contracts-cmake/build/contracts/worbli.reg -p worbli.reg


worbli system newaccount worbli.admin --transfer provider1 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider1 /home/rob/git/compliance-contracts-cmake/build/contracts/provider -p provider1


worbli system newaccount worbli.admin --transfer provider2 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider2 /home/rob/git/compliance-contracts-cmake/build/contracts/provider -p provider2


worbli system newaccount worbli.admin --transfer client EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract client /home/rob/git/compliance-contracts-cmake/build/contracts/client -p client


worbli push action worbli.reg addcred '["identity", "kyc based identity verification"]' -p worbli.reg
worbli push action worbli.reg addcred '["kyc", "kyc based identity verification"]' -p worbli.reg
worbli push action worbli.reg addprovider '["provider1", "Indentity provider"]' -p worbli.reg
worbli push action worbli.reg addprovider '["provider2", "kyc provider"]' -p worbli.reg
worbli push action worbli.reg addprovcred '["provider1", "identity"]' -p worbli.reg
worbli push action worbli.reg addprovcred '["provider2", "kyc"]' -p worbli.reg


