libnss_ec2
==========

EC2 インスタンスの Name タグで Public IP Address を解決できるようになります。

ssh や ping, telnet, mysql など名前解決を利用するアプリケーションで利用できます。

ライブラリを組み込む前に、まずは同封の list コマンドで Public IP Address が出力されるか確認してみてください。

Prebuilt 版のインストール方法(x86-64/ubuntu/trusty ～)
```
sudo cp prebuilt/libnss_ec2.so /lib/x86_64-linux-gnu/libnss_ec2.so.2  # .2 を忘れずに

sudo vi /etc/nsswitch.conf

hosts:          files myhostname dns ec2
# hosts: 行の dns より後ろに ec2 を追加する

```


ビルド方法
----------

CMake3 系や C++11 対応ビルド環境を必要に応じてインストールしてください。

```
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
```

