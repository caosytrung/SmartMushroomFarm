# SmartMushroomFarm
Các chức năng chính
	- Đọc dữ liệu môi trường nhà trồng nấm gửi lên gateway dùng sóng RF
	- Gửi dữ liệu môi trường từ gateway lên server thông qua MQTT
	- Nhận lệnh điều khiển từ server thông qua MQTT
	- Điều khiển tự động máy bơm riêng biệt cho các cụm

AES Lib for arduino & raspberry pi : http://spaniakos.github.io/AES/
Raspberry Chạy project: run file \app\control\demo.py;

Arduino gồm 3 file tương ứng với 3 cụm riêng biệt.

Trong khóa luận đang trình bày sử dụng một máy bơm chia thành 3 vòi phun khác nhau. Mỗi 
vòi phun được lắp với 1 van từ để điều khiển mỗi cụm. Thực tế là sử dụng 3 máy bơm khác nhau. Mỗi 
máy bơm điều khiển cho từng cụm ( vì chưa có thời gian mua vàn từ và giá thành của máy bơm cũng rẻ )

- Ở mỗi cụm hiện tại chỉ bao gồm arduino, 1 module nRf24L01 và 1 module DHT22.
- Cách lắp đặt ở gateway và cụm nấm đã trình bày trong khóa luận.

Note : Phía server được viết bởi Khóa luận tốt nghiệp của bạn "Nguyễn Hoàng Biên - K59 CLC - 2018".
 Server sẽ nhận nhận dữ liệu từ nhà trồng nấm gửi lên và truyền lệnh điều khiển về.