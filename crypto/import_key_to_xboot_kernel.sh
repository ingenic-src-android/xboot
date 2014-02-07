#step1  push boot.img and system.img into crypto dir

#step2 vim Makefile open config example npm701_ab_msc_config
# CONFIG_RSA_VERIFY 1
# CONFIG_THREE_STAGE = y
# CONFIG_RSA_VERIFY = y

#step 3  ./crypto/import_key_to_xboot_kernel.sh

passwd_root=1

cd boot/tools/crypt/
make 


echo "$passwd_root" | sudo -S chmod 755 ./g.sh

./g.sh

cd ../../../

cp boot/tools/crypt/public.binary crypto/
cp boot/tools/crypt/private.key crypto/

echo "$passwd_root" | sudo -S chmod 755 crypto/sign_boot_and_pack.sh crypto/sign_kernel.sh crypto/sign_recovery.sh

# return xboot dir
make

make crypto_kit

make digi_sign

cd crypto 


./sign_kernel.sh

./sign_recovery.sh
cd ..



