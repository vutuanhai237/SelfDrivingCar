Self Driving Car 2018
-- FPT Car Digital Race --
1. File server
2. .vscode
3. lane_detect
a. traffic_sign
Là class dùng để nhận diện biển báo
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
Chúng ta sẽ dùng 2 thư viện này để sử dụng các hàm chuẩn opencv2
Ở src code này chúng tôi đã sử dụng không gian màu IHLS vì thấy nó dễ đặt ngưỡng để nhận diện những vật thể đặc trưng tốt hơn so với không gian màu RGB.
Trong class chúng tôi sẽ khai báo những tham số sau dùng cho:
Scalar(iLowH, iLowL, iLowS): ngưỡng dưới
Scalar(iHighH, iHighL, iHighS): ngưỡng trên
static int CountFrame dùng để đếm số frame xuất hiện liên tục của vật thể (giống như timer)
static const int MinSquare = 200 là kích thước nhỏ nhất dùng để xác định vật thể (mà cụ thể ở đây là biển báo)
static const int LockFlag = 10: sau 10 frame xuất hiện liên tục cùng một vật thể, biến flag sẽ được bật
static const int UnlockFlag = 50 ~ 2s là thời gian hiệu lực của vật thể
static bool flag: được bật khi có vật thể
static int OldSign: xác định vật thể sau có giống vật thể trước hay không
static const int SkyLine; static const int LeftLine: 2 tham số dùng để giới hạn khu vực tìm kiếm vật thể, mà cụ thể ở đây sẽ là góc trên bên phải
static int Sign: trả về loại biển báo (-1 nếu rẽ trái, 1 nếu rẽ phải và 0 nếu không có biển báo)
void Setting(): tùy chỉnh ngưỡng
int Find(const Mat &src): gọi các hàm nhận diện, input là một matrix src, output là giá trị cho biến Sign (trả về nếu cần thiết)
Mat PreFix(const Mat &src): bước tiền xử lý
[
int iIgnoreObj = 1: dùng cho 2 hàm erode và dilate (dùng để khử nhiễu)
cvtColor(des, des, COLOR_BGR2HLS): chuyển không gian màu theo thứ tự Blue, Green, Red sang Hue, Lightness, Saturation. Việc đảo gược thứ tự này giúp Blue -> Red, Green -> Green và Red -> Blue để hàm InRange nhận giá trị không phải bitwise_and hue 0 hue 255
Cuối cùng hàm trả về matrix binary des đã được tiền xử lý
]
Mat ThresholdDetection(Mat &draw, const Mat &Thres): input là matrix RGB draw dùng để vẽ và matrix binary thres dùng để nhận diện vật thể, output là matrix chỉ có vật thể
[
findContours: tìm những vùng có bit 1 trong matrix thres

vòng for tiếp theo duyệt những vùng bit 1 và tìm vùng có diện tích lớn nhất (hay vật thể lớn nhất)
if (rec.area() < MinSquare): nếu vật thể lớn nhất vẫn nhỏ hơn MinSquare thì bỏ qua nhận diện, 
rectangle để vẽ hình chữ nhật bao lấy vùng nhận diện được vật thể trên Mat draw
Mat fix (Thres, rec): tạo matrix binary chỉ có vật thể, sau đó sẽ được ép về hình vuông
Bước cuối cùng là trả về fix
]
int CheckSign(const Mat &src): input là matrix binary vật thể, output là loại biển báo
[ bổ sung sau]


