# Giới thiệu trò chơi

## Các menu
- Main Menu
- Progress
- Setting

## Trò chơi chính

- Giới thiệu về các cơ chế của game
	- Các làn đường, làn nước, làn tàu, skill, debuff, ...
	- Lên level, mở map, mở nhân vật, nâng cấp nhân vật
- Các chế độ chơi
- Các màn chơi + debuff + quái
- Các nhân vật + skill
- Cơ chế lưu và tải của trò chơi

# Các kĩ thuật đã sử dụng trong trò chơi

- Polymorphism
	- Các màn hình inherit từ AbstractScreen
	- Các làn đường trong game
	- ...
- Singleton:
  	- SharedAudio
- Cơ chế hiển thị và vẽ của game
	- Hiển thị hình ảnh, các định dạng đã dùng
	- Main Game loop
		- Pros: đơn luồng, có thể giới hạn fps, viết logic game đơn giản hơn, consitent game speed
		- Cons: code cần chạy nhanh

- Đa luồng
	- Chơi nhạc ở luồng khác
		- Đưa lệnh vào hàng đợi rồi xử lí từ từ
		- Hàng đợi cần có độ dài vừa đủ, tránh cấp phát động


# Video Demo
