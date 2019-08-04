#!/bin/sh

worbli system newaccount worbli.admin --transfer providers EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract providers /home/rob/git/compliance-contracts/contracts/verification_registry/build/verification_registry -p providers


worbli system newaccount worbli.admin --transfer provider1 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider1 /home/rob/git/compliance-contracts/contracts/provider/build/provider -p provider1


worbli system newaccount worbli.admin --transfer provider2 EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract provider2 /home/rob/git/compliance-contracts/contracts/provider/build/provider -p provider2


worbli push action provider1 addentry '["producer2", "identity", true]' -p provider1
worbli push action providers addcred '["identity", "kyc based identity verification"]' -p providers


