Giaii thích class Line_function
line function được sử dụng cho một số class khác
line function được khai báo là một đường thẳng có dạng ax + by + c = 0, ở đây chúng ta không dùng đường thẳng dạng y = ax + b vì nó có dính tới trường hợp y = 0 nên xử lý hơi phức tạp.
class line_function chứa một số hàm tính toán bao gồm:
Line CaculateLine(const Point &a, const Point &b): nhận vào 2 điểm và trả về một đường thẳng đi qua 2 điểm đó
double Distance(const Line &line, const Point &p): nhận vào 1 điểm và 1 đường thẳng, trả về khoảng cách từ điểm đến đường thẳng đó.
double Angle(const Line &line): nhận vào đường thẳng và trả về góc giữa đường thẳng đó với trjc tung
double Angle(const Point &p, const Point &CarLoction = Point(240, 240)): trả về góc giữa đường thẳng tạo bởi 2 điểm và trục tung
int ReturnX(const Line &line, const double &y): trả về giá trị x khi thay giá trị y vào dudongf thẳng
Line Linear(const vector<Point> &p): vẽ đường thẳng đi qua nhiều Point trong vector<Point> nhất có thể
  
