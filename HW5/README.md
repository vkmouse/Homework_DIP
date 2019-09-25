- [實作方法](#實作方法)
- [編譯和執行](#編譯和執行)
- [結果與討論](#結果與討論)

### 實作方法
本次作業實作填滿區域，而且限定在二值化的影像執行。圖1是影像原圖，裡面只包含黑色 (pixel = 0) 與白色 (pixel = 255)。目標是將黑色的點填為白色，將所有白圈中去除黑色點。實作方式是用圖2的component做填滿，首先選取一個點，並以此點為起始點，不斷用component計算，當component為1且該像素是黑色則改為白色，並以此點作為接下來的起點，重複此步驟直到完全填滿。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-1.jpg)|
|:---:|
| 圖1 影像原圖 |

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-3.svg)|
|:---:|
| 圖2 Component |

### 編譯和執行
編譯方式

	g++ main.cpp -o output 'pkg-config --libs opencv'
	
執行時需帶有一個參數，輸入的圖片路徑，例如輸入影像檔名為 pic1.jpg 執行以下命令

	./output pic1.jpg
	
使用滑鼠點擊視窗中黑色位置，即可進行填補。

### 結果與討論

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-1.jpg)|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW5-2.jpg)|
|---|---|
| Original picture | After hole filling |