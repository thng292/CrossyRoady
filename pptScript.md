# Giới thiệu trò chơi

## Các menu (Phước, mở đầu luôn)

- Main Menu
- Progress
- Setting
  - How to play

## Trò chơi chính

- Giới thiệu về các cơ chế của game (Quan)
  - Các làn đường, làn nước, làn tàu, skill, debuff, ...
  - Lên level, mở map, mở nhân vật, nâng cấp nhân vật
- Các chế độ chơi (Quan)
- Các màn chơi + debuff + quái (Vũ)
- Các nhân vật + skill (Vũ)
- Một chút technical detail về cách làm debuff, skill (1 - 2 slide) (Vũ)
- Cơ chế lưu và tải của trò chơi (Vũ)
- Xử lí các làn đường trong game

# Các kĩ thuật đã sử dụng trong trò chơi

- Cơ chế hiển thị và vẽ của game (Thông)

  - Hiển thị hình ảnh, các định dạng đã dùng
  - Main Game loop
    - Các màn hình inherit từ AbstractScreen
    - Pros: đơn luồng, có thể giới hạn fps, viết logic game đơn giản hơn, consitent game speed
    - Cons: code cần chạy nhanh

- Đa luồng (nếu còn thời gian) (Thông)
  - Chơi nhạc ở luồng khác
    - Đưa lệnh vào hàng đợi rồi xử lí từ từ
    - Hàng đợi cần có độ dài vừa đủ, tránh cấp phát động

# Video Demo
