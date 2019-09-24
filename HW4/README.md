- [實作方法](#實作方法)
- [編譯和執行](#編譯和執行)
- [結果與討論](#結果與討論)

### 實作方法
本次作業實作產生與去除雜訊，要產生兩種雜訊分別是胡椒鹽雜訊 (salt-and-pepper noise) 和高斯雜訊 (Gaussian noise)。產生雜訊必須要有一個參數 ![](http://latex.codecogs.com/gif.latex?p)，代表雜訊的比例，胡椒鹽雜訊是將像素改變為 0 或 255，對於人眼是產生一個非常大的對比度，所以非常容易辨識。高斯雜訊則是將像素值增加 ![](http://latex.codecogs.com/gif.latex?N(μ,σ))，以平均值為 ![](http://latex.codecogs.com/gif.latex?μ)，標準差為 ![](http://latex.codecogs.com/gif.latex?σ) 的高斯亂數。圖1分別為原圖和雜訊圖的差別，可以觀察出最為明顯差異的是胡椒鹽雜訊，對於人眼有嚴重的觀看影響。圖中間的高斯雜訊則與原圖只有些微差異，圖中以標準差為10的參數產生高斯雜訊圖。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-6.jpg)|
|:---:|
| 圖1 原圖(左)；高斯雜訊圖(中)；胡椒鹽雜訊圖(右) |

去除雜訊要實作三種濾波器，分別是mean filter、median filter 和 alpha-trimmed filter，詳細步驟如圖2所示。圖中的濾波器大小為 3×3，mean filter 會將濾波器的所有數值執行平均，得到的結果是187，該數值會放入新圖的該位置 (濾波器正中間)。Median filter 會取濾波器數值的中位數，故在實作時必須先執行排序後，取得矩陣中間數值。Alpha-trimmed filter 有另一個需要設定的參數 ![](http://latex.codecogs.com/gif.latex?d)，該參數代表要刪除極端值的數量，故執行時會先排序並刪除頭尾各 ![](http://latex.codecogs.com/gif.latex?\frac{2}{d}) 的數值，再將剩餘數值平均。

|![](https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-7.svg)|
|:---:|
| 圖2、實作三種濾波器詳細步驟 |

### 編譯和執行
編譯方式

	g++ main.cpp -o output 'pkg-config --libs opencv'
	
執行時需帶有一個參數，輸入的圖片路徑，例如輸入影像檔名為 pic1.jpg 執行以下命令

	./output pic1.jpg

### 結果與討論

|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-1.jpg" width="160" height="160" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-2.jpg" width="160" height="160" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-3.jpg" width="160" height="160" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-4.jpg" width="160" height="160" />|<img src="https://github.com/vkmouse/Homework_DIP/blob/master/figure/HW4-5.jpg" width="160" height="160" />|
|:---:|:---:|:---:|:---:|:---:|
| Original picture | Noisy picture | Mean filter | Median Filter | Alpha-trimmed filter |