class lane_detect
static const float alpha: giá trị làm mềm size lane

static int BlurValue;
static int KernelSize;

static int Accuracy: phần trăm chính xác trong detect_lane (0 -> 100%)

static const int SkyLine; // =80
static const Size Box; // 5x5 - Box detetion lane
static double Fx_a, static double Fx_b: hàm f(x) = Fx_a*x+Fx_b - trả về khoảng cách ignore from mid
static const int TooFew: khi số lượng point trong vector<Lane> thấp hơn tooFew thì hiểu là mất lane
Tiếp theo là các vector<Point> Lane
static vector<Point> LaneL;
static vector<Point> LaneR;
static vector<Point> LaneM;
static Mat draw (RGB): dùng để vẽ

void Setting(): cài đặt 2 giá trị blur (2n+1) và accuracy

void Detect(const Mat): dùng để gọi các hàm trong phạm vi lane_detect
void DrawLane(): dùng để vẽ 2 vector LaneL và LaneR
void UpdateMidLane(): cập nhật lại lanesize dựa vào LaneL & LaneR
static void DrawVirtualLane(): vẽ lane ảo dùng khi cua
Mat ReduceNoise(const Mat &src): input và matrix RGB, output là matrix gray đã được giới hạn phạm vi xử lý
Mat CvtBinary(const Mat &src): input là matrix gray, output là matrix binary đã áp dụng giải thuật Laplacian
void FindLane(const Mat &Binary): input là binary matrix
về bản chất, một hình ảnh là một tập hợp ma trận 2 chiều các điểm ảnh với gốc tọa độ là góc trên bên trái

[
Các bước làm:
Ứng với từng đường thẳng y = high, tìm [CẦN VẼ HÌNH]
]
int IgnoreFromMid(int h): INPUT là độ cao, trả về khoảng cách [vẽ hình]
]


