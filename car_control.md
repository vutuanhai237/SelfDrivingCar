static const int LineDetect: tọa độ y của vị trí xe cố gắng đi tới
static const double TurnStrong: độ nghiêng khi vẽ lane ảo
double ReduceAngle(const double &angle): làm mềm góc lái
double GetAngle():
[
có 3 trường hợp như sau:
+ Mất 2 lane:
if (LaneDetect::LaneL.size() + LaneDetect::LaneR.size() < (LaneDetect::TooFew << 1))
trả về góc trước đó
+ Mất 1 trong 2 lane:
++Mất khi có biển báo: tiến hành vẽ lane ảo
++Mất do vật cản, nhiễu, camera không nhìn thấy lane (khi cua gấp): bám theo lane còn lại
+ Đủ 2 lane: 
++ tính toán tạo vector LaneM: L.UpdateMidLane();
++ tính đường thẳng đi qua vector LaneM: res = F.Linear(LaneDetect::LaneM);
++ nếu không có biển báo: cập nhật kích thước đường
++ tính góc dựa theo vị trí xe hiện tại và điểm cắt giữa đường thẳng res và LineDetect
]
int CarControl::GetSpeed(const double & angle): trả về tốc độ dựa theo góc lái, nếu góc lái càng lớn,
tức cua càng gấp thì tốc độ phải giảm, qua nhiều lần thực nghiệm, chúng tôi thấy công thức trên là an toàn nhất với nhiều trường hợp.



