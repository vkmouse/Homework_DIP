- [實作方法](#實作方法)
- [編譯和執行](#編譯和執行)
- [結果與討論](#結果與討論)

### 實作方法
本次作業實作影像轉換，將影像執行變形，如圖1所示，整體流程共有五個部分，第一個步驟是先將準備好的影像讀入程式中，第二步驟是輸入四個位置，這些位置是要變形成為矩形的位置。第三個步驟是計算八參數的數值，第四步驟則是雙線性插值法 (Bilinear Interpolation) 插值，最後顯示出插值後的圖片。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW2-3.svg)|
|:---:|
| 圖1 影像轉換流程圖 |

八參數的使用方式是如公式1,2，![](http://latex.codecogs.com/gif.latex?a_1)~![](http://latex.codecogs.com/gif.latex?a_8) 是八個要計算的系數，![](http://latex.codecogs.com/gif.latex?u) 是新的圖片的 x 軸，![](http://latex.codecogs.com/gif.latex?v) 是新的圖片的 y 軸；![](http://latex.codecogs.com/gif.latex?x) 是原始圖片的 x 軸，![](http://latex.codecogs.com/gif.latex?y) 是原始圖片的 y 軸，前一步驟選擇的四個點分別帶入![](http://latex.codecogs.com/gif.latex?uvxy)，即可列出八個方程式計算所有系數，得到系數後再依序推回新圖的點映射到原始圖的點，補上插值法增加精細度。

![](http://latex.codecogs.com/gif.latex?x=a_1\times%20uv+a_2\times%20u+a_3\times%20v+a_4}) ---- 公式1

![](http://latex.codecogs.com/gif.latex?y=a_5\times%20uv+a_6\times%20u+a_7\times%20v+a_8}) ---- 公式2


### 編譯和執行
編譯方式

	g++ main.cpp -o output 'pkg-config --libs opencv'
	
執行時需帶有一個參數，輸入的圖片路徑，例如輸入影像檔名為 pic.jpg 執行以下命令

	./output pic1.jpg

### 結果與討論


|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW2-1.jpg?raw=true)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW2-2.jpg)|
|---|---|
| Original picture | Wrapping picture |