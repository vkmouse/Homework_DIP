- [實作方法](#實作方法)
- [編譯和執行](#編譯和執行)
- [結果與討論](#結果與討論)

### 實作方法
本次作業實作非銳化濾鏡 (unsharp masking) 的影像銳化，比起直接針對高頻進行銳化的方法，非銳化濾鏡能夠準確的在邊緣進行銳化，減少對雜訊的方大。如圖1所示，整體流程共有六個部分，第一步驟是先將準備好的影像讀入程式中，第二步驟是透過Laplacian Filter計算二階導數的影像，第三步驟透過兩個維度的一階導數，並把計算結果相加而得Sobel 的影像，第四步驟是用平均的方式模糊化Sobel 的影像，主要目的是去除雜訊，第五步驟是將其正規化至0 ~ 1(除以255) 再乘上二階導數的結果，讓二階邊緣偵測去雜訊，第六步驟將去雜訊的邊緣偵測疊合至原圖產生一張銳化的結果。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW3-3.svg)|
|:---:|
| 圖1 銳化流程圖 |


### 編譯和執行
編譯方式

	g++ main.cpp -o output 'pkg-config --libs opencv'
	
執行時需帶有一個參數，輸入的圖片路徑，例如輸入影像檔名為 pic1.jpg 執行以下命令

	./output pic1.jpg

### 結果與討論

|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW3-1.jpg" width="177" height="266" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW3-2.jpg" width="177" height="266" />|
|---|---|
| Original picture | After image sharpening |