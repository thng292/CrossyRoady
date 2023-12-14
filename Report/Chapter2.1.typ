== Các cơ chế hiển thị hình ảnh

Để hiển thị các pixel vuông trên màn hinh console, em đã lấp đầy màn hình console bằng kí tự "\u{2584}" (U+2584) (Lower Half Block). Lúc này, mỗi dòng chữ trên màn hình console sẽ trở thành 2 hàng pixel hình vuông. Để đổi màu của pixel phía trên, chỉ cần đổi màu nền của console tại kí tự đó, còn muốn đổi màu pixel phía dưới, chỉ cần đổi màu chứ của console tại kí tự đó. 

Để dễ dàng vẽ các hình ảnh lên, em dùng thêm một mảng phụ như một màn hình ảo, rồi từ màn hình ảo đó vẽ lên màn hình thật. Màn hình ảo này có độ phân giải là 384 #sym.times 224 mỗi pixel trên màn hình ảo có chiều dài và rộng bằng 3 pixel trên màn hình thật và có thể hiển thị được 16 màu.

Trong code, màn hình ảo này được truy cập thông qua lớp thuần ảo sau:
```Cpp
class AbstractCanvas {
   public:
    virtual Vec2 CanvasSize() const = 0;
    virtual Color* operator[](size_t index) = 0;
    virtual void Clear(Color color) = 0;
};
```

Lớp gôm các phương thức:
- `CanvasSize`: Trả về độ lớn của màn hình ảo dưới dạng vector.
- `operator[]`: Sử dụng để truy cập vào từng pixel của màn hình ảo, hàm trả về con trỏ để có thể truy xuất như mảng 2 chiều.
- `Clear`: Thay toàn bộ pixel trên màn hình thành màu `color`

== Màu sắc

Vì màn hình console chỉ hiển thị được 16 màu, đã vậy, các màu còn không hợp với chủ đề của trò chơi. Tuy nhiên, ta có thể đổi được 16 màu ấy thành 16 màu khác. Chúng em đã lợi dụng cơ chế này, để hiển thị thêm nhiều màu sắc khác trên màn hình console.

Để thuận tiện trong việc phát triển, chúng em lưu trữ bảng màu trong các file văn bản có đuôi .hex, mỗi màu một dòng. Khi cần, chương trình sẽ đọc file và đổi thành bảng màu mới.

== Cách lưu trữ hình ảnh

Với số lượng hình ảnh lớn và độ phân giải khá cao, việc lưu trữ hình ảnh trực tiếp trong code là không còn khả thi nữa. Đồng thời để hỗ trợ cho việc vẽ hình ảnh từ các công cụ bên ngoài, chúng em đã chọn sử dụng một định dạng ảnh đơn giản. Ngoài ra, để tương thích với nhiều thiết bị hơn, các số nguyên nhiều byte luôn được lưu theo Big Endian

=== Lưu trữ hình ảnh tĩnh

Hình ảnh tĩnh được lưu dưới định dạng sau:
#table(
  columns: (1fr, 1fr),
  [*Trường*], [*Độ dài*],
  [Chiều cao], [2 byte],
  [Chiều rộng], [2 byte],
  [Dữ liệu], [Dài #sym.times Rộng byte]
)

Phần dữ liệu là màu sắc của điểm ảnh, được lưu sử dụng 1 byte. Ngoài ra còn một giá trị đặc biệt là `0xF1` để biểu hiện sự trong suốt.

Bên dưới là ví dụ một file ảnh tĩnh trong trò chơi. Hình, đã lược bỏ chiều dài, rộng và in dưới dạng hex thêm màu

#figure(caption: [Ví dụ một file ảnh tĩnh trong trò chơi])[
  #image("asset/bushHex.png",width: 60%)
],

#figure(caption: [File ảnh được hiển thị trong trò chơi])[
  #image("asset/bushGame.png",width: 60%)
] 

=== Lưu trữ hình ảnh động

Hình ảnh động được lưu dưới định dạng sau:
#table(
  columns: (1fr, 1fr),
  [*Trường*], [*Độ dài*],
  [Chiều cao], [2 byte],
  [Chiều rộng], [2 byte],
  [Số khung hình của ảnh], [4 byte],
  [Số khung hình trên giây], [1 byte],
  [Dữ liệu], [Dài #sym.times Rộng byte #sym.times Số khung hình]
)

Phần dữ liệu của ảnh động cũng tương tự như ảnh tĩnh, nhưng thay vì một ảnh thì sẽ là nhiều ảnh được lưu liên tiếp nhau.

Để hiển thị ảnh động trong trò chơi, ta lấy tổng số khung hình chia cho số khung hình trên giây để tính được thời gian giữa 2 khung hình. Từ đó sẽ xác định được nên vẽ khung hình nào lên màn hình


=== Lưu trữ Font chữ

Vì không có nhiều kinh nghiệm trong việc xử lí font chữ, nên chúng em đã chọn lưu font chữ tương tự như cách lưu ảnh. Một file font chữ sẽ bao gồm các kí tự ASCII từ 33 tới 126 được đặt liên tiếp xếp chồng lên nhau. Lúc cần hiển thị, chương trình sẽ tự cắt và lấy ra kí tự phù hợp

Có 2 font chữ chính được sử dụng trong trò chơi:
  - Font chữ lớn: Cao 13, ngang 8
  - Font chữ nhỏ: Cao 9, ngang 6
#grid(
  columns: (1fr, 1fr), 
  figure(caption: [Font chữ nhỏ])[
    #image("asset/smallFont.png")
  ],
  figure(caption: [Font chữ lớn])[
    #image("asset/bigFont.png")
  ],
)

=== Công cụ hỗ trợ

Để dễ dàng hơn trong việc sử dụng các công cụ vẽ, chúng em đã phát triển một công cụ nhỏ, giúp chuyển đổi từ các định dạng hình ảnh phổ biến như png, jpeg thành định dạng riêng của trò chơi. Ngoài ra, công cụ còn có thể chuyển một thư mục ảnh thành định dạng ảnh động của trò chơi.

Công cụ có tên là spritegen2, được viết bằng golang, cách sử dụng như sau:
```txt
Usage: spritegen <filename|dir> [options] 
Options:   
  -o <filename|dir>   Output filename   
  -m <mode>           Mode: dir | ani | single(default) | watch   
  -fps <mode>         Animation's FPS   
  -c <filename>       Color palette file  
```

Một vài ví dụ sử dụng:
  - Chuyển hình ảnh "forest.png" sang thành định dạng hình ảnh sử dụng trong trò chơi tên "preview.sprite" sử dụng bảng màu nằm trong file "forest.hex"
    ```shell-unix-generic
    spritegen2 forest.png -c forest.hex -o preview.sprite 
    ```
  - Ghép tất cả hình ảnh trong thư mục "anime" thành định dạng ảnh động sử dụng trong trò chơi tên "mob1R.anisprite" sử dụng bảng màu nằm trong file "forest.hex", ảnh động có số khung hình trên giây là 10
    ```shell-unix-generic
    spritegen2 anime -m ani –fps 10 -c forest.hex -o mob1R.anisprite 
    ```

== Vòng lặp của game

=== Ý tưởng

Cách chia nhiều luồng khác nhau để xử lí các làn xe nghe đơn giản, nhưng khi thực hiện thì có nhiều bất cập như:
- Khó quản lí trạng thái của trò chơi do phải chia sẻ dữ liệu giữa các luồng.
- Khó để thêm được các tính năng như hiệu ứng môi trường và kĩ năng nhân vật.
- Khó debug.

Vì vậy, chúng em chọn không chia tành nhiều luồn, mà các logic sẽ được tập trung và chạy một cách đồng bộ trên một luồng duy nhất. Cách làm này mang lại nhiều lợi ích như:

- Việc quản lí trạng thái của trò chơi sẽ đơn giản hơn nhiều do không cần phải chia sẻ trạng thái cho nhiều luồng khác nhau.
- Giảm thiểu được việc tranh giành các tài nguyên chung như màn hình, từ đó tránh được các lỗi lạ liên quan đến xung đột tài nguyên.
- Logic game đơn giản hơn và dễ debug hơn.
- Tốc độ của trò chơi sẽ ổn định hơn và ít phụ thuộc vào tài nguyên của máy.


Để thực hiện ý tưởng trên, đầu tiên chúng em định nghĩa các giai đoạn của một vòng lặp và vai trò của từng bước.

Mỗi lần lặp, game sẽ thực hiện các bước sau:
  + _Kiểm tra đầu vào của người dùng: _
    - Kiểm tra các nút và cập nhật trạng thái của chúng, đây là giai đoạn duy nhất kiểm tra đầu vào của người dùng, các phím mà người dùng nhấn sau đó sẽ không được ghi nhận.
    - Để kiểm tra nút có được ấn hay không, chúng em sử dụng hàm `GetAsyncKeyState(int vKey)`, hàm này sẽ kiểm tra xem nút có được ấn hay không tại thời điểm gọi và trả về ngay lập tức chứ không chờ người dùng ấn nút.
  + _Cập nhật trạng thái trò chơi: _
    - Lúc này, trò chơi sẽ xử lí đầu vào của người dùng và cập nhật trạng thái tương ứng. 
  + _Vẽ trạng thái mới lên màn hình: _
    - Sau khi tính được trạng thái mới, trò chơi cập nhật trạng thái mới lên màn hình.

    
=== Chi tiết thực hiện

==== Ngăn xếp màn hình

Khi bắt đầu, trò chơi sẽ khởi tạo một ngăn xếp chứa con trỏ tới lớp AbstractScreen (@baseScreen) để quản lí các màn hình có trong trò chơi. Tiếp theo, game sẽ gọi constructor của tất cả các man hinh có trong trò chơi và lưu vào một mảng. Mỗi khi cân tạo màn hinh mới đê cho vào ngân xếp, game sẽ tim màn hinh trong mảng và dùng phương thức `Clone` để tạo màn hình mới.

Màn hình đầu tiên ở trên stack sẽ là màn hình chính, chỉ có màn hình này được cập nhật và vẽ lên màn hình.

==== AbstractNavigation

Lớp trừ tượng này dùng để tạo ra yêu cầu chuyển đổi màn hình. Lớp sẽ được đưa vào hàm cập nhật của màn hình. Khi cần chuyển đổi sang màn hình khác, màn hình hiện tại sẽ gọi tới các phương thức của lớp này và cung cấp các thông tin cần thiết. Lớp sẽ tại ra gói tin để thực hiện yêu cầu đó, còn nếu không có nhu cầu chuyển đổi thì sẽ trả về gói tin tạo ra từ phương thức `NoChange`

Lớp bao gồm các hàm:
  - `NoChange`: Tạo gói tin không bao gồm yêu cầu nào
  - `Back(payload)`: Tạo gói tin mang yêu cầu quay lại màn hình nằm trước nó trong ngăn xếp màn hình. Có thể chuyển dữ liệu cho màn hình trước đó thông qua `payload`.
  - `Navigate(screenName, payload)`: Tạo gói tin mang yêu cầu đưa màn hình có tên `screenName`. Có thể chuyển dữ liệu cho màn hình chính mới thông qua `payload`.
  - `PopBackTo(screenName, payload)`: Tạo gói tin mang yêu cầu quay lại màn hìnhcó tên `screenName` trong ngăn xếp màn hình. Có thể chuyển dữ liệu cho màn hình đó thông qua `payload`.
  - `Exit`: Tạo gói tin mang yêu cầu thoát khỏi chương trình.

Dưới đây là chữ kí của lớp
```Cpp
class AbstractNavigation {
 public:
  enum class NavigationAction : char {
      Back,
      Navigate,
      PopBackTo,
      Exit,
      None,
  };

  struct NavigationRes {
      NavigationAction ActionType;
      std::wstring_view ActionData;
      std::any Payload;
  };

  virtual NavigationRes NoChange() const = 0;

  virtual NavigationRes Back(std::any payload) const = 0;

  virtual NavigationRes Navigate(
      std::wstring_view screenName, std::any payload
  ) const = 0;

  virtual NavigationRes PopBackTo(
      std::wstring_view screenName, std::any payload
  ) const = 0;

  virtual NavigationRes Exit() const = 0;
}
```

==== AbstractScreen <baseScreen>

```cpp
class AbstractScreen {
   public:
    virtual std::wstring_view getName()     = 0;

    virtual void Init(const std::any& args) = 0;
    virtual AbstractScreen* Clone() const   = 0;
    virtual void Mount(const std::any& args
    ) {};
    virtual NavigationRes Update(
        float deltaTime,
        const AbstractNavigation* navigation
    ) = 0;
    virtual void Draw(AbstractCanvas* canvas) const = 0;
    virtual void Unmount() {};
};
```

Đây là lớp cơ sở cho tất cả các màn hình trong trò chơi. Các màn hình sẽ kế thừa từ lớp này và ghi đè các lớp cần thiết.

Ý nghĩa của từng phương thức:
 - `getName`: lấy tên của màn hình. Mỗi màn hình sẽ có một tên đặc biệt và duy nhất dùng để quản lí lúc chuyển đổi màn hình.
 - `Init`: Hàm này chỉ được gọi một lần duy nhất khi chuyển đến từ một màn hình khác, màn hình trước đó, nếu cần có thể chuyển dữ liệu cho màn hình mới chuyển đến. Lúc này, dữ liệu đó sẽ được truyền vào hàm này thông qua tham số `args`
 - `Clone`: Hàm này dùng để tạo ra một bản sao của màn hình hiện tại. Hàm được gọi mỗi khi ngăn xếp cần tạo ra màn hình mới.
 - `Mount`: Hàm này có thể sẽ được gọi nhiều lần. Hàm được gọi khi màn hình trở thành màn hình chính, dùng để tải các tài nguyên cần thiết để hiển thị màn hình. Khi màn hình chính thực hiện hành động quay về, muốn truyền dữ liệu cho màn hình chính tiếp theo thì dữ liệu đó sẽ được truyền vào hàm thông qua tham số `args`
 - `Update`: Hàm này được gọi liên tục, dùng để cập nhật trạng thái của màn hình. Hàm được truyền thêm 2 tham số là `deltaTime`: Khoảng thời gian giữa lần gọi hàm update này và lần trước đó. `navigation`: Lớp hỗ trợ tạo gói tin chuyển đổi màn hình. Hàm sẽ trả về gói tin chuyển đổi màn hình được tạo bởi `navigation`
 - `Draw`: Hàm này được gọi liên tục, dùng để vẽ trạng thái hiện tại của màn hình lên màn hình console. Hàm sẽ được truyền cho một `canvas` để vẽ lên.
 - `Unmount`: Hàm này được gọi khi màn hình chính chuyển thành màn hình khác, các hoạt động như giải phóng tài nguyên tạm thời sẽ được thực thi ở đây.

== Chơi âm thanh trên luồng khác

Game xử lí việc chơi âm thanh bằng hàm `mciSendString`, hàm này có một điểm yếu là: thời gian xử lí không đều, lúc nhanh, lúc chậm. Vì trò chơi chỉ chạy trên một luồng, nên các công việc không biết trước thời gian chạy như vậy có thể làm chậm cả trò chơi và gây ra trải nghiệm khó chịu cho người chơi.

Để giải quyết vấn đề này, nhóm chúng em chọn cách: gọi hàm `mciSendString` trên một luồng khác.

Ban đầu, với mỗi lần gọi hàm `mciSendString`, game sẽ tạo ra một luồng mới để chạy. Nhưng có một vấn đề là các tài nguyên mở bằng hàm này sẽ được gắn với luồng gọi nó. Nói cách khác, khi mở tài nguyên trên một luồng, thì tài nguyên ấy chỉ có thể được truy cập trên luồng đó.

Điều này đưa chúng em đến một giải pháp khác: chạy tất cả các lệnh `mciSendString` trên một luồng và sử dụng hàng đợi để giảm thiểu độ trễ khi chạy trò chơi.

Trò chơi lúc này sẽ có 2 luồng: một luồng chính để chạy trò chơi, và một luồng phụ để chơi âm thanh.

Nếu luồng chính muốn chơi âm thanh, thì sẽ tạo lệnh rồi đưa vào hàng đợi và thông báo cho luồng phụ. Lúc này luồng phụ sẽ thức dậy và thực hiện các lệnh trong hàng đợi cho tới khi hàng đợi rỗng rồi quay về trạng thái chờ.

Cách giải quyết này sẽ thêm một chút độ trễ khi cùng lúc gửi nhiều lệnh vào hàng đợi.