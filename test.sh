#!/bin/sh

worbli system newaccount worbli.admin --transfer providers EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract providers /home/rob/git/compliance-contracts/contracts/worbli.compliance/build/worbli.compliance -p providers


worbli system newaccount worbli.admin --transfer provider1 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider1 /home/rob/git/compliance-contracts/contracts/provider/build/provider -p provider1


worbli system newaccount worbli.admin --transfer provider2 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider2 /home/rob/git/compliance-contracts/contracts/provider/build/provider -p provider2


worbli system newaccount worbli.admin --transfer test EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract test /home/rob/git/compliance-contracts/contracts/test/build/test -p test


worbli push action providers addcred '["identity", "kyc based identity verification"]' -p providers
worbli push action providers addcred '["kyc", "kyc based identity verification"]' -p providers
worbli push action providers addprovider '["provider1", "Indentity provider"]' -p providers
worbli push action providers addprovider '["provider2", "kyc provider"]' -p providers
worbli push action providers addprovcred '["provider1", "identity"]' -p providers
worbli push action providers addprovcred '["provider2", "kyc"]' -p providers


#worbli push action provider1 addentry '["producer2", "kyc", true]' -p provider1
#worbli push action provider1 addentry '["producer2", "identity", true]' -p provider1

#worbli push action test hi '["producer2"]' -p test



