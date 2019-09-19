- [實作方法](#實作方法)
- [編譯和執行](#編譯和執行)
- [結果與討論](#結果與討論)

### 實作方法
本次作業實作影像轉換，如圖1所示，整體流程共有四個部分，第一個步驟是先將準備好的影像讀入程式中，第二步驟是輸入一個要轉換的比例，而作業中限定為將原圖放大為10倍，第三個步驟則是分別用兩種插值法補放大後缺失的數值，這兩種插值法分別為最近鄰居 (Nearest Neighbor Interpolation) 與雙線性插值法 (Bilinear Interpolation)，最後顯示出插值後的圖片。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-4.svg)|
|:---:|
| 圖1 影像轉換流程圖 |

在實作過程中，使用的方式是反向映射 (Backward Mapping)，概念如圖2左側兩張圖，是由放大圖片的個別像素映射到原始圖片，並利用插值法計算該像素的數值。右上角的示意圖是最近鄰居插值法，其方式是計算該點的最近鄰居，並且把該點的像素值設定與最近鄰居相同；右下角的示意圖是雙線性插值法，該插值法所補的像素值會與最近的四個點相關，利用最近的四個點依照距離比例計算該像素值，如圖中雙向插值法補的數值就會接近最近鄰居的顏色，但並完全相同，相異則是來自於另外三個較遠點的顏色。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-5.svg)|
|:---:|
| 圖2 影像轉換概念 |

### 編譯和執行
編譯方式

	g++ main.cpp -o output 'pkg-config --libs opencv'
	
執行時需帶有一個參數，輸入的圖片路徑，例如輸入影像檔名為 pic.jpg 執行以下命令

	./output pic1.jpg

### 結果與討論

從實驗結果中可以得知最近鄰居法產生的圖可以明顯看出像素方塊的形狀，是較差的放大結果，而雙線性插補法放大的結果卻沒有明顯的顆粒狀，比較好的放大方法，但由於將圖放置word上也同時會縮小圖片的比例，故看起來差異並沒有非常明顯，若從原始放大圖中可以看到更加明顯的差異。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-1.jpg)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-2.jpg)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW1-3.jpg)|
|---|---|:---:|
| Original picture | Nearest neighbor interpolation | Bilinear interpolation |