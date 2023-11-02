#set page(
  paper: "a4",
  header: align(right)[
    Báo cáo lần 1 nhóm 12
  ],
  numbering: "1",
)

#set heading(
    numbering: "1.a)",
)

#set text(
    font: "Time News Roman",
    size: 12pt
)

#place(
    center + horizon,
    block(
        width: 100%,
        height: 100%,
        stroke: 2pt,
        place(
            center + horizon,
            [
            #align(center, text(32pt, pad(rest: 32pt)[
                *Báo cáo đồ án môn học\
                lần 1 của nhóm 12*
            ]))

            #align(center)[
                22127401 - Nguyễn Quang Thông\
                22127341 - Nguyễn Thế Quan\
                22127339 - Võ Nhật Phước\
                22127463 - Nguyễn Anh Vũ            
            ]

            #pad(
                rest: 32pt,
                align(center)[
                *Giáo Viên Hướng Dẫn: Trương Toàn Thịnh*
            ])
            ]
        )
    )
)


#pagebreak()
#heading(numbering: none)[Mục lục]
#outline(
    title: none,
    indent: true
)

#pagebreak()

= Nhiệm vụ đã giao:

== Tuần 2

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Viết code xử lí màn hình, tốc độ khung hình, công cụ vẽ], [Công cụ vẽ hoạt động tốt, tốc độ khung hình 60 fps], [13-19/10], [Hoàn thành tốt],
    [Võ Nhật Phước], [Tìm nhạc cho game], [Tìm được nhạc nền, nhạc khi chơi, nhạc khi bị thua], [13-19/10], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Tìm thiết kế nhân vật, map chơi], [Thiết kế đẹp, có hoạt ảnh], [13-19/10], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Tìm font chữ], [Font chữ phù hợp với màn hình], [13-19/10], [Hoàn thành tốt],
)


== Tuần 3

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Sửa các lỗi liên quan tới màn hình], [Màn hình phù hợp tới kích thước nhân vật, vừa di chuyển và update màn hình], [20-26/10], [Hoàn thành tốt],
    [Võ Nhật Phước], [Thiết kế logic game], [Có sơ đồ ý tưởng về logic game], [20-26/10], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Vẽ nhân vật, map, hiệu ứng của nhân vật, hoạt ảnh], [Đẹp, đúng kích thước được giao], [20-26/10], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Load font chữ lên màn hình], [In được lên màn hình với nhiều màu, đúng kích thước], [20-26/10], [Hoàn thành tốt],
)


== Tuần 4

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Load ảnh nhân vật động lên màn hình, tìm hiểu về chơi mạng LAN], [Load dược nhân vật trên màn hình], [27/10-2/11], [Hoàn thành tốt],
    [Võ Nhật Phước], [Xử lí các chướng ngại vật], [Đúng ý tưởng nhóm đề ra], [27/10-2/11], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Chỉnh sửa, bổ sung thêm nhân vật, hiệu ứng, map], [Mới lạ, đẹp mắt], [27/10-2/11], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Viết báo cáo lần 1], [Hoàn thanh báo cáo], [27/10-2/11], [Hoàn thành tốt],
)

= Các tính năng đã hoàn thành:
- Hình vẽ cho nhân vật
- Đề ra các định dạng file nhị phân
- Hoàn thành phần code để vẽ và quản lí màn hình

= Kế hoạch cho tương lai:
- Hoàn thành các menu: main menu, setting, pause,...
- Thống nhất các âm thanh trong game
- Hoàn thiện sơ bộ phần game
- Vẽ UML cho các lớp sẽ sử dụng
