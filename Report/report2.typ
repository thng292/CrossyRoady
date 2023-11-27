#set page(
  paper: "a4",
  header: align(right)[
    Báo cáo lần 2 nhóm 12
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
                lần 2 của nhóm 12*
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

== Tuần 5

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Viết code xây dựng main menu, màn hình setting], [Màn hình setting có thể tùy chọn tắt mở nhạc], [3-9/11], [Hoàn thành tốt],
    [Võ Nhật Phước], [Viết code vẽ sông, đường,...], [Đầy đủ các loại đường cho từng loại map], [3-9/11], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Viết code vẽ các chướng ngại vật], [Chướng ngại vật đủ cho từng loại map],[3-9/11], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Viết code vẽ nút tương tác trên giao diện], [Thiết kế đẹp, đổi được màu, kích thước], [3-9/11], [Hoàn thành tốt],
)


== Tuần 6

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Viết code màn hình Characters], [Giao diện đẹp, đầy đủ thông tin từng nhân vật], [10-16/11], [Hoàn thành tốt],
    [Võ Nhật Phước], [Viết code xử lí màn hình khi nhân vật di chuyển], [Đúng ý tưởng nhóm đề ra], [10-16/11], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Viết code cho chướng ngại vật di chuyển], [Đầy đủ cho các chế độ chơi], [10-16/11], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Viết code màn hình trong main menu: cách chơi, thông tin nhóm,... ], [Giao diện đẹp], [10-16/11], [Hoàn thành tốt],
)


== Tuần 7

#table(
    columns: (auto, auto, auto, auto, auto),
    inset: 5pt,
    align: horizon,
    [*Thành viên*], [*Công Việc*], [*Yêu Cầu*], [*Thời gian*], [*Đánh giá*],
    [Nguyễn Quang Thông], [Viết code về tính năng nâng cấp nhân vật], [Đúng ý tưởng nhóm đề ra], [17-23/11], [Hoàn thành tốt],
    [Võ Nhật Phước], [Viết code hiển thị và tương tác của các vật phẩm với nhân vật], [Đúng ý tưởng nhóm đề ra], [17-23/11], [Hoàn thành tốt],
    [Nguyễn Anh Vũ], [Viết code màn hình chọn nhân vật trước khi bắt đầu chơi], [Mới lạ, đẹp mắt], [17-23/11], [Hoàn thành tốt],
    [Nguyễn Thế Quan], [Viết báo cáo lần 2], [Hoàn thanh báo cáo], [17-23/11], [Hoàn thành tốt],
)

= Các tính năng đã hoàn thành:
- Các menu: main menu, setting, pause,...
- Có nhạc nền, nhạc khi chơi, tương tác,...
- Hoàn thiện cơ bản phần game

= Kế hoạch cho tương lai:
- Vẽ UML cho các lớp sẽ sử dụng
- Làm Powerpoint để thuyết trình
