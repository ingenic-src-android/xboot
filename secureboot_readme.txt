本文描述了x-boot secureboot的使用

2012.2.7

简介
目前实现了使用libtom的rsa_verify进行数字签名校验kernel和stage 3 bootloader的功能。

更改：
为了实现上述功能，更改包括：
1. 移植libtomcrypt, tomsfastmath.
2. 修改bootloader到可以支持三阶段boot的过程。主要修改是产生了boot/second和boot/third目录。
3. 修改使用了xbootimg的文件头，以便增加数字签名和public key

使用:
目前在Makefile里面已经加入了方便的命令，可以很容易使用secureboot的功能。

1. 生成rsa公私密钥对
cd boot/tools/crypt
make
./g.sh
生成的publickey.binary拷贝到boot目录，private.key拷贝到crypto目录

2. 修改config，配置rsa校验和三级启动，这个修改在顶层Makefile里面，比如：
 945 npm701_ab_msc_config: unconfig
……
 958         @echo "#define CONFIG_RSA_VERIFY 1" >> include/config.h
……
 969         @echo "CONFIG_THREE_STAGE = y" >> include/config.mk
 970         @echo "CONFIG_RSA_VERIFY = y" >> include/config.mk
……
添加以上配置

3. 编译
make
现在可以安全的使用make -j4加速编译
生成mbr-xboot.bin和x-boot3.bin
因为按照目前考虑的使用，mbr-xboot.bin是放在OTP区的，包涵了第一第二级的bootloader，以及public.binary
而x-boot3.bin则包涵了第三阶段的代码，以及代码的数字签名

4. 构建加密工具目录
make crypto_kit
这样顶层目录crypto所需的工具就构建好了
make digi_sign
生成打包的三级bootloader，在crypto/boot.bin，原来烧写mbr-xboot.bin的地方修改为烧写boot.bin

5. 对kernel image进行数字签名
把kernel的映象boot.img拷贝到crypto目录，生成boot_signed.img，原来烧写boot.img的地方改为烧写boot_signed.img即可。

6. recovery.cpio.img进行数字签名
由于recovery.cpio.img存在于system.img中，因此需要把system.img拷贝到crypto目录，然后执行./sign_recovery.sh
该脚本会mount system.img，然后进行签名
这个过程中，需要root权限，如果存在passwd文件在crypto目录中，则可以直接执行，不需要中途输入密码。
passwd文件需要包含当前用户的密码，并且以换行结尾，也就是这个文件需要第二行是一个空行。

#####
如果需要快速编译请看crypto下import_key_to_xboot_kernel.sh
#####
