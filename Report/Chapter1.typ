= Tổng quan về trò chơi

==  Giới thiệu về trò chơi

=== Trò chơi băng qua đường (Crossy Roady) 

Một số tính năng đặc biệt của trò chơi
    - Người chơi sẽ được chọn nhân vật, mỗi nhân vật đều có một kĩ năng riêng để hỗ trợ trong quá trình chơi
    - Có 6 bản đồ khác nhau, ban đầu người chơi có một bản đồ mặc định, lên cấp để mở các bản đồ tiếp theo
    - Mỗi bản đồ là một môi trường có chủ đề khác nhau, có nhạc nền riêng và tác động lên người chơi theo nhiều cách khác nhau
    - Mỗi bản đồ có 4 cấp độ: dễ, trung bình, khó và tự động (điều chỉnh độ khó dựa trên điểm của người chơi)
    - Mỗi bản đồ có 3 loại quái khác nhau với 3 độ khó khác nhau, thiệt hại khác nhau
    - Để tăng sự kịch tính, trò chơi sẽ giới hạn bên dưới di chuyển lên dần, người chơi phải luôn giữu nhân vật của mình bên trên
    - Trong quá trình chơi, nhân vật có thể nhặt những vật phẩm rơi trên đường đi
    - Có nhiều loại đường đi khác nhau
    - Có thể sử dụng chuột để điều hướng trong trò chơi
    
#pagebreak()
=== Các yêu cầu về tính năng
    - Có thể lưu, tải trò chơi đang chơi
    - Xử lí giao diện màn hình khi chơi
    - Xử lí màn hình chính
    - Người chơi có thể bât/tắt các tính năng cơ bản, có thể lưu lại các thiết lập
    - Đa dạng nhiều chế độ chơi
    - Có nhiều bản đồ với chủ đề khác nhau
    - Xử lí các làn đường khác nhau

=== Thông tin chung về trò chơi
#figure(
    caption: "Thông tin chung về trò chơi",
    table(
        columns: (auto, auto),
        inset: 8pt,
        [*Tên trò chơi*], [Crossy Roady],
        [*Môi trường phát triển và thử nghiệm*], [Visual Studio 2022],
        [*Khả dụng trên nền tảng*], [Windows],
        [*Video Demo sản phẩm*], link("https://www.youtube.com/watch?v=tCnMEVvIxhU"),
        [*Source code*], link("https://github.com/thng292/CrossyRoady")
    )
)

#pagebreak()
== Giao diện trong trò chơi

=== Menu chính
Đây là "Menu Chính" của trò chơi. Người chơi có thể chọn các chức năng khác nhau bằng cách chọn vào các mục tương ứng. Các mục chức năng bao gồm:
    - Một người chơi: Chế độ một người chơi.
    //- Hai người chơi: Chế độ hai người chơi.
    - Nhân vật: Chọn nhân vật cho trò chơi.
    - Cài đặt: Tùy chỉnh các thiết lập của trò chơi.
    - Thoát: Thoát trò chơi.

#figure(
    image("asset/MainMenu.png", width: 100%),
    caption: [Main Menu]
)

=== Cài đặt
Gồm 4 muc:
- Bật tắt âm thanh nền
- Bật tắt âm thanh hiệu ứng
- Hướng dẫn chơi
- Credit
#figure(
    image("asset/setting.png", width: 100%),
    caption: [Setting]
)
=== Hướng dẫn 
- Người chơi có thể dùng các nút A, W, S, D hoặc các nút lên, xuống, trái, phải để di chuyển; F hoặc Enter để dùng kĩ năng và esc để dừng game

#figure(
    image("asset/control.png", width: 100%),
    caption: [Control]
)

- Khi chơi game sẽ nhận được điểm kinh nghiệm để lên cấp
- Ở mỗi bản đồ nếu đủ mức điểm tối thiểu của bản đồ yêu cầu có thể nhận nhân vật mới
-- Mức dễ: 400
-- Mức trung bình: 300
-- Mức khó: 100
-- Chế độ tự động: 200
#figure(
    image("asset/exp.png", width: 100%),
    caption: [Exp]
)
- Sau khi nâng cấp nhân vật, kĩ năng của nhân vật ấy sẽ trở nên mạnh mẽ hơn
#figure(
    image("asset/upgrade.png", width: 100%),
    caption: [Upgrade]
)
=== Progress
- Bản thông tin tiến trình cung cấp cho người chơi biết về: thời gian đã chơi, số lần chết, số bước đã đi, kinh nghiệm hiện có, cấp độ, số nhân vật đã mở khóa, số kĩ năng đã nâng cấp và số bản đồ chơi đã mở
#figure(
    image("asset/progress.png", width: 100%),
    caption: [Progress]
)
=== Giao diện màn hình khi chơi
Giao diện màn hình khi chơi trò chơi là một phần rất quan trọng của trò chơi vì nó là nơi mà người chơi dành nhiều thời gian tương tác với trò chơi nhất. Khi thiết kế màn hình này, chúng em đã phải suy nghĩ rất kĩ, trên màn hình sẽ hiện thông tin nào, hiện ra sao và sắp xếp như thế nào. Cuối cùng, chúng em đã thiết kế nó như sau:

#figure(
    image("asset/GamePlay.png", width: 100%),
    caption: [Giao diện màn hình khi chơi]
)

== Các cơ chế trong game

=== Nhân vật
Người chơi có thể chọn nhân vật cho mình khi tạo trò chơi mới. Có tổng cộng 6 nhân vật với 6 kĩ năng khác nhau, mỗi kĩ năng sẽ có những tương tác đặc biệt tới bản đồ khi kích hoạt. Màn hình chọn nhân vật sẽ hiển thị thông tin về tên nhân vật, số máu, tốc độ di chuyển, thông tin kĩ năng.
#figure(
    image("asset/AvatarSelect.png", width: 100%),
    caption: [Chọn nhân vật]
)

==== Ceres Fauna
Kĩ năng: Hồi đầy máu cho bản thân và tăng máu tạm thời thêm 2 máu.
#figure(
    image("asset/fauna.png", width: 30%),
    caption: [Ceres Fauna]
)
==== Irys
Kĩ năng: Nhận 2 tấm khiên giúp cho nhân vật va chạm với quái vật không mất máu, tiêu hao 1 khiên khi bị va chạm.
#figure(
    image("asset/irys.png", width: 30%),
    caption: [Irys]
)
==== Nanashi Mumei
Kĩ năng: Trong một khoảng thời gian ngắn trở nên bất khả xâm phạm, đi xuyên qua quái vật không bị mất máu và tăng tốc độ di chuyển.
#figure(
    image("asset/mumei.png", width: 30%),
    caption: [Nanashi Mumei]
)
==== Ouro Kronii
Kĩ năng: Dừng mọi chuyển động trên bản đồ.
#figure(
    image("asset/kronii.png", width: 30%),
    caption: [Ouro Kronii]
)
==== Tsukumo Sana
Kĩ năng: Loại bỏ hiệu ứng debuff đang phải chịu.
#figure(
    image("asset/sana.png", width: 30%),
    caption: [Tsukumo Sana]
)
==== Hakos Baelz
Kĩ năng: Nhận ngẫu nhiên một trong 5 kĩ năng của các nhân vật hoặc bị đảo chiều di chuyển.
#figure(
    image("asset/baelz.png", width: 30%),
    caption: [Hakos Baelz]
)

=== Bản đồ
Để tránh người chơi cảm thấy nhàm chán khi phải chơi mãi một bản đồ, trò chơi cung cấp cho người chơi 6 bản đồ với 6 chủ đề khác nhau. Trong mỗi bản đồ, sẽ có sự xuất hiện của các loại quái vật mà người chơi cần tránh, có thể xuất hiện quái vật đi bộ hay các đoàn quái vật chạy nhanh. Ngoải ra, với việc có 6 bản đồ chơi với 6 chủ đề khác nhau thì cảnh vật sẽ vô cùng đa dạng, phong phú và nhiều màu sắc. Đặc biệt hơn là mỗi bản đồ sẽ có một tính năng được gọi là debuff có thể tùy chọn tắt mở, tính năng này sẽ tương tác với nhân vật trong quá trình chơi.
#figure(
    image("asset/Map.png", width: 100%),
    caption: [Chọn bản đồ]
)

==== Forest
Debuff: Nếu nhân vật đứng yên trong 3 giây sẽ bị mất 1 máu.
#figure(
    image("asset/forest.png", width: 40%),
    caption: [Forest]
)
==== City
Debuff: Tạm thời khóa máu nhân vật xuống còn 1, trong thời gian bị debuff không thể tăng máu dù bằng cách nào.
#figure(
    image("asset/city.png", width: 40%),
    caption: [City]
)
==== Haunted House
Debuff: Giảm tầm nhìn của nhân vật bằng cách làm tối khu vực xung quanh nhân vật trong khoảng thời gian ngắn.
#figure(
    image("asset/house.png", width: 40%),
    caption: [Haunted House]
)
==== Desert
Debuff: Ngưng chuyển động của nhân vật trong vài giây.
#figure(
    image("asset/desert.png", width: 40%),
    caption: [Desert]
)
==== Space
Debuff: Không cho phép nhân vật dùng kĩ năng, xóa hiệu ứng kĩ năng hiện tại của nhân vật đang sử dụng.
#figure(
    image("asset/space.png", width: 40%),
    caption: [Space]
)
==== Casino
Debuff: Nhận ngẫu nhiên một trong 5 debuff của các bản đồ hoặc đảo chiều di chuyển của nhân vật.
#figure(
    image("asset/casino.png", width: 40%),
    caption: [Casino]
)
=== Vật phẩm
Trong quá  trình chơi sẽ xuất hiện những vật phẩm khác nhau trên bản đồ, nhân vật có thể nhặt chúng và sử dụng như những kĩ năng thêm, tùy thuộc vào vật phẩm nhặt được sẽ có những khả năng của riêng chúng.
#figure(
    image("asset/HP.png", width: 40%),
    caption: [Vật phẩm hồi máu]
)
#figure(
    image("asset/Skill.png", width: 40%),
    caption: [Vật phẩm hồi kĩ năng]
)
#figure(
    image("asset/Boost.png", width: 40%),
    caption: [Vật phẩm tăng tốc]
)
=== Kinh nghiệm và lên cấp
==== Mở khóa bản đồ
Để tăng tính thử thách cho người chơi, hệ thống ban đầu chỉ mở khóa bản đồ đầu tiên là Forest, nhiệm vụ của người chơi là thu thập điểm kinh nghiệm qua mỗi lần chơi. Người chơi cần kiếm đủ 800 điểm kinh nghiệm để lên cấp trong Progress. Tăng cấp trong Progress sẽ mở khóa bản đồ chơi mới.
#figure(
    image("asset/UnlockMap.png", width: 100%),
    caption: [Màn hình thông báo mở khóa bản đồ]
)
==== Mở khóa nhân vật
Cũng giống như cơ chế mở khóa bản đồ, hệ thống cũng sẽ chỉ mở khóa nhân vật đầu tiên là Fauna cho người chơi. Người chơi sẽ nhận được một nhân vật mới ở mỗi bản đồ nếu chỉ trong một lần chơi có thể đạt được đủ số điểm yêu cầu. Số điểm yêu cầu sẽ tùy thuộc vào độ khó mà người chơi chọn.

#figure(
    caption: "Điểm mở khóa nhân vật",
    table(
        columns: (auto, auto),
        inset: 8pt,
        [*Tự động*], [200 điểm],
        [*Dễ*], [400 điểm],
        [*Bình thường*], [300 điểm],
        [*Khó*], [100 điểm],
    )
)
#figure(
    image("asset/UnlockChar.png", width: 100%),
    caption: [Màn hình thông báo mở khóa nhân vật]
)
==== Nâng cấp nhân vật
Khi lên cấp trong Progress, hệ thống không chỉ mở khóa bản đồ mới mà tùy thuộc vào cấp độ sẽ cho người chơi "Điểm nâng cấp nhân vật". Điểm nâng cấp nhân vật giúp cho nhân vật của người chơi hồi kĩ năng nhanh hơn khi chơi và giới hạn mỗi nhân vật chỉ được nâng cấp một lần.
#figure(
    image("asset/UpgradeChar.png", width: 100%),
    caption: [Màn hình nâng cấp nhân vật]
)
=== Các mức độ khó
Trò chơi gồm có 4 độ khó: Dễ, Trung bình, Khó, Tự động - độ khó tăng dần theo mức điểm. Việc có nhiều độ khó như vậy sẽ giúp người chơi làm quen với trò chơi một cách dễ dàng hơn. Ứng với mỗi độ khó sẽ xuất hiện những loại quái gây thiệt hại khác nhau cho nhân vật. Với việc có 6 loại bản đồ khác nhau thì các loại quái của từng bản đồ cũng sẽ có hình dạng khác nhau phù hợp với chủ đề bản đồ đó.
#figure(
    image("asset/Diff.png", width: 100%),
    caption: [Màn hình chơi chế độ giới hạn thời gian]
)
=== Các chế độ chơi
==== Chế độ không giới hạn
Người sẽ chơi đến khi nào máu của nhân vật trở về 0 hoặc nhân vật bị đưa ra ngoài phạm vi bản đồ.

==== Chế độ giới hạn thời gian
Đối với chế dộ giới hạn thời gian, trò chơi sẽ có thêm 3 lụa chọn giới hạn: 5 phút, 10 phút, 15 phút. Khi bắt đầu trò chơi với chế độ này, phía bên trên góc trái màn hình sẽ xuất hiện một đồng hồ đếm ngược, nếu đồng hồ này đếm ngược về 0 thì trò chơi sẽ kết thúc.
#figure(
    image("asset/ModeScreen.png", width: 100%),
    caption: [Màn hình chọn chế độ chơi]
)
#figure(
    image("asset/LimitedMode.png", width: 100%),
    caption: [Màn hình chơi chế độ giới hạn thời gian]
)
=== Lưu trò chơi
Trò chơi chỉ cung cấp tính năng lưu một màn chơi. Khi người chơi đã chọn lưu màn đang chơi hiện tại, trong lần chọn tạo trò chơi mới tiếp theo sẽ xuất hiện dòng thông báo thông tin về màn chơi đã lưu, nếu người dùng chọn "Yes" thì hệ thống sẽ đưa người chơi quay lại màn chơi đó là tiếp tục phần chơi của mình như bình thường.
#figure(
    image("asset/SaveGameScreen.png", width: 100%),
    caption: [Màn hình thông báo màn chơi đã lưu]
)