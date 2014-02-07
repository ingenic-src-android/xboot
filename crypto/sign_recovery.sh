mkdir -p mnt
#if [ -e passwd ] ;then
#	cat passwd | sudo -S mount -o loop -t ext4 system.img mnt || exit 1;
#else
#	sudo mount -o loop -t ext4 system.img mnt || exit 1
#fi

passwd_root=1
echo "$passwd_root" | sudo -S mount -o loop -t ext4 system.img mnt || exit 1
echo "$passwd_root" | sudo -S ./sign_kernel mnt/recovery.cpio.img -o mnt/recovery.cpio.img 
echo "$passwd_root" | sudo -S umount mnt
