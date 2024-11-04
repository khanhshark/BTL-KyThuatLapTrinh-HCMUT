// TODO:
// Chỉnh lại di chuyển của RobotC (gợi ý: thực hiện hàm getPrevPosition cho Criminal)
// Mô tả về các meet thay đổi (đã được confirm 90%)
// File study_in_pink2.h những phần trước "addition" là gốc của đề (không thay đổi)
// Chỉnh tên thuộc tính hp và exp của Sherlock và Watson
// Position có nạp chồng 2 hàm isEqual
// isStop kiểm tra vị trí Sherlock, Watson với Criminal và hp của Sherlock, Watson
// Hàm run chú ý chỉnh từ hàm run() gốc (vị trí gọi printResult và printStep)
// Hàm move của Sherlock và Watson khi exp == 0 sẽ không thực hiện gì
// NOTE:
// chú ý các phần addition nên sửa đổi để tránh đạo code
// nộp Bkel cần xóa đổi lại 2 hàm printResult và printStep gốc, xóa thuộc tính outputFile
#include "study_in_pink2.h"
// !!----------------------------------------------------------------
// ! UPDATE 26/5: cập nhật meet theo logic endGame và bỏ tham số robotType, di chuyển của robot và create tạo theo chuẩn logic move trước tạo robot sau
// ! RobotSW chỉ còn đi UU RR DD LL
// !------------------------------------------------------------------
// ! Nội dung task cũ cần bỏ vào (chỉ bỏ dòng #include "moving.h")
// ! Các mục sửa đổi:
// ! HÀM KHỞI TẠO: Sherlock và Watson thêm thuộc tính bag, Robot thêm thuộc tính để sinh item
const Position Position::npos = Position(-1, -1);
/*
 * CLASS: MapElement
 */
MapElement::MapElement(ElementType in_type)
{
    // TODO: constructor
    type = in_type;
    // cout <<" a ";
}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    // TODO: get
    return type;
}
/*
 * CLASS: Path
 */
Path::Path()
    : MapElement(PATH) {}
/*
 * CLASS: Wall
 */
Wall::Wall()
    : MapElement(WALL) {}
/*
 * CLASS: FakeWall
 */
FakeWall::FakeWall(int in_req_exp)
    : MapElement(ElementType::FAKE_WALL)
{
    // TODO: constructor
    req_exp = in_req_exp;
}
int FakeWall::getReqExp() const
{
    // TODO: get
    return req_exp;
}
/*
 * CLASS: Position
 */

Position::Position(int r, int c)
{
    // TODO: constructor
    // cout << " 6 ";
    this->r = r;
    this->c = c;    
}
Position::Position(const string &str_pos)
{
    // TODO: constructor
    size_t start = str_pos.find('(') + 1;
    size_t end = str_pos.find(',');
    r = stoi(str_pos.substr(start, end - start));

    start = end + 1;
    end = str_pos.find(')');
    c = stoi(str_pos.substr(start, end - start));
    // cout << "asssssssssssssssssssssssssssssss";
}
int Position::getRow() const
{
    // TODO: get
    return r;
}
int Position::getCol() const
{
    // TODO: get
    return c;
}
void Position::setRow(int r)
{
    // TODO: set
    this->r = r;
}
void Position::setCol(int c)
{
    // TODO: set
    this->c = c;
}
string Position::str() const
{
    // TODO: trả về chuỗi "(<r>,<c>)"
    return "(" + to_string(r) + "," + to_string(c) + ")";
}
bool Position::isEqual(Position position) const
{ 
    // TODO: so sánh tọa độ với Position khác
    return r == position.r && c == position.c;
}
bool Position::isEqual(int in_r, int in_c) const
{
    return r == in_r && c == in_c;
}

/*
 * CLASS: MovingObject
 */
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    // TODO: constructor
    this->index = index;    
    this->pos = pos;    
    this->map = map;
    this->name = name;
}

Position MovingObject::getCurrentPosition() const
{       
    // TODO: get
    return pos;
}
Position MovingObject::getNextPosition(){
    Position nextPos = this->pos;
    if(nextPos.isEqual(Position::npos)){
        nextPos = Position::npos;
    }
    return nextPos;
}
void MovingObject::move() {
    Position nextPos = getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    } 
}
string MovingObject::str() const {
    return name + "[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}
MovingObjectType MovingObject::getObjectType() const {
    return MovingObjectType::SHERLOCK;
}
/*
 * CLASS: Character kế thừa class MovingObject
 */
Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(index, pos, map, name)
{
    // TODO: constructor
    
}
/*
 * CLASS: Sherlock kế thừa class Character
 */



Position Sherlock::getNextPosition()
{
    // TODO: tính vị trí tiếp theo dựa theo nước đi kế tiếp
    if(this->getEXP() == 0){
        return Position::npos;
    }
    char direction = moving_rule[index_moving_rule];
    Position nextPos = pos;
    switch(direction){
        case 'U':
            nextPos.setRow(nextPos.getRow() - 1);
            break;
        case 'D':
            nextPos.setRow(nextPos.getRow() + 1);
            break;
        case 'L':
            nextPos.setCol(nextPos.getCol() - 1);
            break;
        case 'R':
            nextPos.setCol(nextPos.getCol() + 1);
            break;
    }
   
    if(!map->isValid(nextPos, this)){
        nextPos = Position::npos;
    }
    return nextPos;
}
void Sherlock::move()
{
    
    // TODO: di chuyển đến vị trí tiếp theo và cập nhật nước đi tiếp theo
    Position nextPos = Sherlock::getNextPosition();
    if (!nextPos.isEqual(Position::npos))
    {
        this->pos = nextPos;
    }
    index_moving_rule = (index_moving_rule + 1) % moving_rule.length();
}
string Sherlock::str() const
{
    //  TODO: trả về chuỗi "Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]"
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

MovingObjectType Sherlock::getObjectType() const
{
    // TODO: get
    return MovingObjectType::SHERLOCK;
}

int Sherlock::getHP() const
{
    // TODO: get
    if(hp > 500){
        return 500;
    }
    else if(hp < 0){
        return hp;
    }
    else{
        return hp;
    }
}
int Sherlock::getEXP() const
{
    // TODO: get
    if(exp > 900){
        return 900;
    }
    else if(exp < 0){
        return 0;
    }
    else{
        return exp;
    }
}
void Sherlock::setHP(int hp)
{
    // TODO: set
    this->hp = hp;
}

void Sherlock::setEXP(int exp)
{
    // TODO: set
    this->exp = exp;
}
Sherlock::~Sherlock(){
    delete bag;
}
/*
 * CLASS: Watson kế thừa class Character
 */


Position Watson::getNextPosition()
{
    // TODO: tính vị trí tiếp theo dựa theo nước đi kế tiếp
    if(this->getEXP() == 0){
        return Position::npos;
    }
    char direction = moving_rule[index_moving_rule];
    Position nextPos = pos;
    switch(direction){
        case 'U':
            nextPos.setRow(nextPos.getRow() - 1);
            break;
        case 'D':
            nextPos.setRow(nextPos.getRow() + 1);
            break;
        case 'L':
            nextPos.setCol(nextPos.getCol() - 1);
            break;
        case 'R':
            nextPos.setCol(nextPos.getCol() + 1);
            break;
    }
   
    if(!map->isValid(nextPos, this)){
        nextPos = Position::npos;
    }
    return nextPos;
}

void Watson::move()
{
    // TODO: di chuyển đến vị trí tiếp theo và cập nhật nước đi tiếp theo
    // cout << "Current index watson: " << moving_rule[index_moving_rule] << " " << endl;
    Position nextPos = Watson::getNextPosition();
    if (!nextPos.isEqual(Position::npos))
    {
        this->pos = nextPos;
    }
     index_moving_rule = (index_moving_rule + 1) % moving_rule.length();
}
string Watson::str() const
{
    //  TODO: trả về chuỗi "Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]"
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

MovingObjectType Watson::getObjectType() const
{
    // TODO: get
    return MovingObjectType::WATSON;
}
// int Watson::getDistance(const Position &pos) const{
//     return abs(this->pos.getRow() - pos.getRow()) + abs(this->pos.getCol() - pos.getCol());
// }
int Watson::getHP() const
{
    // TODO: get
    if(hp > 500){
        return 500;
    }
    else if(hp < 0){
        return 0;
    }
    else{
        return hp;
    }
}

int Watson::getEXP() const
{
    // TODO: get
    if(exp > 900){
        return 900;
    }
    else if(exp < 0){
        return 0;
    }
    else{
        return exp;
    }
}

void Watson::setHP(int hp)
{
    // TODO: set
    this->hp = hp;
}

void Watson::setEXP(int exp)
{
    // TODO: set
    this->exp = exp;
}
Watson::~Watson(){
    delete bag;
}
/*
 * CLASS: Map
 */
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
{
    // TODO: constructor
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    if(num_cols == 0 || num_rows == 0)
        return;
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++)
        {
            map[i][j] = new Path();
        }
    }
    for (int i = 0; i < num_fake_walls; i++)
    {
        int r = array_fake_walls[i].getRow();
        int c = array_fake_walls[i].getCol();
        if (r >= 0 && r < num_rows && c >= 0 && c < num_cols) {
            delete map[r][c];
            map[r][c] = new FakeWall((((r * 257 + c * 139) + 89) % 900) + 1);
        } else {
            // cout << "Invalid position for FakeWall at (" << r << ", " << c << ")\n";
        }
    }
    for (int i = 0; i < num_walls; i++)
    {  
        int r = array_walls[i].getRow();
        int c = array_walls[i].getCol();
        if (r >= 0 && r < num_rows && c >= 0 && c < num_cols) {
            // cout << "Creating Wall at (" << r << ", " << c << ")\n";
            delete map[r][c];
            map[r][c] = new Wall();
        } else {
            // cout << "Invalid position for Wall at (" << r << ", " << c << ")\n";
        }
    }
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (map[i][j]->getType() == WALL) {
                // std::cout << 'W';
            } else if (map[i][j]->getType() == FAKE_WALL) {
                // std::cout << 'F';
            } else {
                // std::cout << '.';
            }
        }
    // std::cout << std::endl;
    }
}
Map::~Map()
{
    // TODO: destructor
    
    if(num_cols == 0 || num_rows == 0)
        return;
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    // TODO: kiểm tra đối tượng có thể di chuyển vào vị trí hay không
    if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols)
        return false;
    if(mv_obj->getObjectType() != ROBOT && mv_obj->getObjectType() != CRIMINAL && mv_obj->getObjectType() != SHERLOCK && mv_obj->getObjectType() != WATSON)
        return false;
    if (map[pos.getRow()][pos.getCol()]->getType() == WALL)
        return false;
    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL)
    {
        if (mv_obj->getObjectType() == WATSON){
            Watson* watson = static_cast<Watson*>(mv_obj);
            int reqEE = ((FakeWall *)map[pos.getRow()][pos.getCol()])->getReqExp();
            if(watson->getEXP() <= ((FakeWall *)map[pos.getRow()][pos.getCol()])->getReqExp()){
                return false;
            }
            return true;
        }    
        else if(mv_obj->getObjectType() == SHERLOCK || mv_obj->getObjectType() == CRIMINAL || mv_obj->getObjectType() == ROBOT){
            return true;
        }
    }
    return true;
}
/*
 * CLASS: Criminal kế thừa class Character
 */

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(index, init_pos, map, "Criminal")
  
{
    // TODO: constructor
    this->sherlock = sherlock;
    this->watson = watson;
    this->count = 0;
    this->prev_pos = Position::npos;
}
int Criminal::manhattanDistance(const Position &pos1, const Position &pos2) const{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
}
Position Criminal::getNextPosition()
{
    // TODO: tính vị trí tiếp theo dựa theo nước đi kế tiếp
    Position currentPos = this->getCurrentPosition();
    Position nextPos = Position::npos;
    int maxDist = -1;
    pair<int, int> directions[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; //ULDR
    for (int i = 0; i < 4; i++) {
        const auto &dir = directions[i];
        Position newPos(currentPos.getRow() + dir.first, currentPos.getCol() + dir.second);
        if (this->map->isValid(newPos, this)) {
            int d1 = manhattanDistance(newPos, sherlock->getCurrentPosition());
            int d2 = manhattanDistance(newPos, watson->getCurrentPosition());
            if (d1 + d2 > maxDist) {
                maxDist = d1 + d2;
                nextPos = newPos;
            }
        }
    }
    if(maxDist == -1){
        nextPos = Position::npos;
    }
    return nextPos;
    // TODO: tìm vị trí tiếp theo gần Watson nhất
}
Position Criminal::getPrevPosition() const{
    return prev_pos;
}
void Criminal::move()
{
    // TODO: di chuyển đến vị trí tiếp theo và cập nhật nước đi tiếp theo
    
    Position nextPos = Criminal::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->prev_pos = this->getCurrentPosition();
        this->pos = nextPos;
        this->count++;
    }
}

string Criminal::str() const
{
    // TODO: trả về chuỗi "Watson[index=<index>;pos=<pos>]"
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}


MovingObjectType Criminal::getObjectType() const
{
    // TODO: get
    return MovingObjectType::CRIMINAL;
}
int Criminal::getCount() const
{
    // TODO: get
    return count;
}
bool Criminal::isCreatedRobotNext() const //! bỏ
{
    if(getCount() % 3 == 2 && getCount() > 0){
        return true;
    }
    return false;
}

/*
 * CLASS: Robot kế thừa class MovingObject
 */

// ItemType Robot::getItem(){
//     return item;
// }
BaseItem *Robot::NewItem(){
    return item;
}
Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    // TODO: trả về đối tượng class kế thừa từ Robot phù hợp
    // ArrayMovingObject *arr = dynamic_cast<ArrayMovingObject*>(map);
    // if(criminal->isCreatedRobotNext()){
    if(criminal->getCount() == 3){
        return new RobotC(index, criminal->getPrevPosition(), map, criminal);
    }
    else{
        //Cach 1
        
        if(criminal->manhattanDistance(criminal->getPrevPosition(), sherlock->getCurrentPosition()) > criminal->manhattanDistance(criminal->getPrevPosition(), watson->getCurrentPosition())){
            return new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
        }                   
        else if(criminal->manhattanDistance(criminal->getPrevPosition(), sherlock->getCurrentPosition()) < criminal->manhattanDistance(criminal->getPrevPosition(), watson->getCurrentPosition())){
            return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        }
        else if(criminal->manhattanDistance(criminal->getPrevPosition(), sherlock->getCurrentPosition()) == criminal->manhattanDistance(criminal->getPrevPosition(), watson->getCurrentPosition())){
            return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        } 
    }
    // }
    return nullptr;
}
Robot::~Robot(){
    delete item;
}
MovingObjectType Robot::getObjectType() const
{
    // TODO: get
    return MovingObjectType::ROBOT;
}
Position Robot::getNextPosition(){
    Position nextPos = this->pos;
    if(nextPos.isEqual(Position::npos)){
        return Position::npos;
    }
    return nextPos;
}
void Robot::move() {
    Position nextPos = Robot::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    }
}
string Robot::str() const{
    return "Robot[pos=" + pos.str() + ";type=" + to_string(robot_type) + ";dist=" + to_string(getDistance()) + "]";
}
RobotType Robot::getType() const{
    return robot_type;
}
int Robot::getDistance() const{
    return 0;
}
/*
 *CLASS: RobotC kế thừa class Robot
 */
RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : Robot(index, init_pos, map, criminal)
          
{
    // TODO: constructor
}

int RobotC::getDistance(Sherlock *sherlock)
{
    // TODO: get
    return this->criminal->manhattanDistance(this->pos, sherlock->getCurrentPosition());
}
int RobotC::getDistance(Watson *watson)
{
    // TODO: get
    return this->criminal->manhattanDistance(this->pos, watson->getCurrentPosition());
}
// TODO implement các phương thức getNextPosition, move, str, getType, getDistance
Position RobotC::getNextPosition()
{
    Position nextPos = Position::npos;
    if(!(criminal->getNextPosition()).isEqual(Position::npos)){
        nextPos = criminal->getPrevPosition();
    }
    return nextPos;
}
void RobotC::move()
{
    // TODO: di chuyển theo Criminal

    Position nextPos = this->RobotC::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    }
}
string RobotC::str() const
{
    // TODO: trả về chuỗi "RobotC[pos=<pos>;type=C;dist=<distance>]"
    return "Robot[pos=" + pos.str() + ";type=C;dist=" + "]";
}
RobotType RobotC::getType() const
{
    // TODO: get
    return RobotType::C;
}
/*
 * CLASS: RobotW kế thừa class Robot
 */

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(index, init_pos, map, criminal)
       
{
    // TODO: constructor
    this->watson = watson;
  
}

Position RobotW::getNextPosition()
{
    // TODO: tìm vị trí tiếp theo gần Watson nhất
    Position currentPos = this->getCurrentPosition();
    Position watsonNextPos = this->watson->Watson::getCurrentPosition();
    int minDist = INT_MAX;
    Position nextPos = Position::npos;
    Position positions[4] = {
        Position(currentPos.getRow() - 1, currentPos.getCol()),     // U
        // Position(currentPos.getRow() - 1, currentPos.getCol() + 1), // UR
        Position(currentPos.getRow(), currentPos.getCol() + 1),     // R
        // Position(currentPos.getRow() + 1, currentPos.getCol() + 1), // DR
        Position(currentPos.getRow() + 1, currentPos.getCol()),     // D
        // Position(currentPos.getRow() + 1, currentPos.getCol() - 1), // DL
        Position(currentPos.getRow(), currentPos.getCol() - 1),     // L
        // Position(currentPos.getRow() - 1, currentPos.getCol() - 1)  // UL
    };
    for(int i = 0; i < 4; i++){
        if(this->map->isValid(positions[i], this)){
            int dist = this->criminal->manhattanDistance(positions[i], watsonNextPos);
            if(dist < minDist){
                minDist = dist;
                nextPos = positions[i];
            }
        }
    }
    return nextPos;
}

void RobotW::move()
{
    // TODO: di chuyển về phía Watson
    Position nextPos = RobotW::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    }
}

string RobotW::str() const
{
    // TODO: trả về chuỗi "RobotW[pos=<pos>;type=W;dist=<distance>]"
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}

RobotType RobotW::getType() const
{
    // TODO: get
    return RobotType::W;
}

int RobotW::getDistance() const
{
    // TODO: get
    return this->criminal->manhattanDistance(this->pos, this->watson->getCurrentPosition());
}
/*
 * CLASS: RobotS kế thừa class Robot
 */
RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(index, init_pos, map, criminal)
{
    // TODO: constructor
    this->sherlock = sherlock;
}

Position RobotS::getNextPosition()
{
    // TODO: tìm vị trí tiếp theo gần Sherlock nhất
    Position currentPos = this->getCurrentPosition();
    Position sherlockNextPos = this->sherlock->Sherlock::getCurrentPosition();

    int minDistance = INT_MAX;
    Position nextPos = Position::npos;

    // Positions around the current position in the clockwise order starting from Up (U, R, D, L)
    Position positions[4] = {
        Position(currentPos.getRow() - 1, currentPos.getCol()),     // U
        // Position(currentPos.getRow() - 1, currentPos.getCol() + 1), // UR
        Position(currentPos.getRow(), currentPos.getCol() + 1),     // R
        // Position(currentPos.getRow() + 1, currentPos.getCol() + 1), // DR
        Position(currentPos.getRow() + 1, currentPos.getCol()),     // D
        // Position(currentPos.getRow() + 1, currentPos.getCol() - 1), // DL
        Position(currentPos.getRow(), currentPos.getCol() - 1),     // L
        // Position(currentPos.getRow() - 1, currentPos.getCol() - 1)  // UL
    };

    for (int i = 0; i < 4; i++) {
        if (this->map->isValid(positions[i], this)) {
            int distance = this->criminal->manhattanDistance(positions[i], sherlockNextPos);
            if (distance < minDistance) {
                minDistance = distance;
                nextPos = positions[i];
            }
        }
    }

    return nextPos;
}
void RobotS::move()
{
    // TODO: di chuyển về phía Sherlock
    Position nextPos = RobotS::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    }
}

string RobotS::str() const
{
    // TODO: trả về chuỗi "RobotS[pos=<pos>;type=S;dist=<distance>]"
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";
}

RobotType RobotS::getType() const
{
    // TODO: get
    return RobotType::S;
}

int RobotS::getDistance() const
{
    // TODO: get
    return this->criminal->manhattanDistance(this->pos, this->sherlock->getCurrentPosition());
}
/*
 * CLASS: RobotSW kế thừa class Robot
 */
RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(index, init_pos, map, criminal)
{
    // TODO: constructor
    this->sherlock = sherlock;
    this->watson = watson;  
}

// TODO implement các phương thức getNextPosition, move, str, getType, getDistance
Position RobotSW::getNextPosition()
{
    // TODO: tìm vị trí tổng giá trị khoảng cách Sherlock và Watson nhỏ nhất
    Position currentPos = this->getCurrentPosition();
    Position sherlockNextPos = this->sherlock->getCurrentPosition();
    Position watsonNextPos = this->watson->getCurrentPosition();

    int minSum = INT_MAX;
    Position nextPos = Position::npos;

    // Positions around the current position in the clockwise order starting from Up (U, R, D, L)
    Position positions[8] = {
        Position(currentPos.getRow() - 2, currentPos.getCol()), // U
        Position(currentPos.getRow() - 1, currentPos.getCol() + 1), // UR
        Position(currentPos.getRow(), currentPos.getCol() + 2), // R
        Position(currentPos.getRow() + 1, currentPos.getCol() + 1), // DR
        Position(currentPos.getRow() + 2, currentPos.getCol()), // D
        Position(currentPos.getRow() + 1, currentPos.getCol() - 1), // DL
        Position(currentPos.getRow(), currentPos.getCol() - 2),  // L
        Position(currentPos.getRow() - 1, currentPos.getCol() - 1)  // UL
    };

    for (int i = 0; i < 8; i++) {
        if (this->map->isValid(positions[i], this)) {
            int sum = this->criminal->manhattanDistance(positions[i], sherlockNextPos) + this->criminal->manhattanDistance(positions[i], watsonNextPos);
            // cout << "RobotSW: " << positions[i].str() << endl;
            // cout << "Distance to sherlock: " << "Sherlock: " << sherlockNextPos.str() << " dist: " << this->criminal->manhattanDistance(positions[i], sherlockNextPos) << endl;
            // cout << "Distance to Watson: " << "Watson: " << watsonNextPos.str() << " dist: " << this->criminal->manhattanDistance(positions[i], watsonNextPos) << endl;
            // cout << "Sum: " << sum << endl;
            if (sum < minSum) {
                minSum = sum;
                nextPos = positions[i];
            }
        }
    }
    return nextPos;
}
string RobotSW::str() const
{
    // TODO: trả về chuỗi "RobotS[pos=<pos>;type=S;dist=<distance>]"
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(RobotSW::getDistance()) + "]";
}
RobotType RobotSW::getType() const
{
    // TODO: get
    return RobotType::SW;
}
int RobotSW::getDistance() const
{
    // TODO: get tổng khoảng cách đến Sherlock và Watson
    return this->criminal->manhattanDistance(this->pos, this->sherlock->getCurrentPosition()) + this->criminal->manhattanDistance(this->pos, this->watson->getCurrentPosition());   
}
void RobotSW::move()
{
    // TODO: di chuyển đến gần cả Sherlock và Watson
    Position nextPos = RobotSW::getNextPosition();
    if(!nextPos.isEqual(Position::npos)){
        this->pos = nextPos;
    }
}
/*
 * CLASS: ArrayMovingObject
 */

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    // TODO: constructor
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[capacity];
    
}
ArrayMovingObject::~ArrayMovingObject(){
    for(int i = 3; i < count; i++){
        delete arr_mv_objs[i];
    }
    if(capacity > 0){
        delete[] arr_mv_objs;
    }
}
bool ArrayMovingObject::isFull() const
{
    // TODO: kiểm tra số lượng đối tượng đã đầy hay chưa
    return count >= capacity;                                    //true if full, false if not full
}
bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    // TODO: thêm đối tượng mới vào cuối mảng đối tượng
    if (!isFull())
    {
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    }
    return false;
}
MovingObject *ArrayMovingObject::get(int index) const
{
    // TODO: trả về đối tượng có index tương ứng
    if (index < 0 || index >= count)
    {
        return nullptr;
    }
    return arr_mv_objs[index];
}

int ArrayMovingObject::size() const
{
    // TODO: trả về số lượng đối tượng trong mảng
    return count;
}

string ArrayMovingObject::str() const
{
    // TODO: trả về chuỗi biểu diễn mảng
    string result = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for (int i = 0; i < count; i++)
    {
        result += ";" + arr_mv_objs[i]->str();
    }
    result += "]";
    return result;
}

/*
 * CLASS: Configuration
 */

Configuration::Configuration(const string &filepath)
{
    //    TODO: constructor
    ifstream file(filepath);
    string line;
    while(getline(file, line)){
        //for int 
        if(line.substr(0,12) == "MAP_NUM_ROWS"){
            map_num_rows = stoi(line.substr(13));
        }
        // cout << "924";
        if(line.substr(0,12) == "MAP_NUM_COLS"){
            map_num_cols = stoi(line.substr(13));
        }
        if(line.substr(0,22) == "MAX_NUM_MOVING_OBJECTS"){
            max_num_moving_objects = stoi(line.substr(23));
        }
        if(line.substr(0,16) == "SHERLOCK_INIT_HP"){
            sherlock_init_hp = stoi(line.substr(17));
        }
        if(line.substr(0,17) == "SHERLOCK_INIT_EXP"){
            sherlock_init_exp = stoi(line.substr(18));
        }
        if(line.substr(0,14) == "WATSON_INIT_HP"){
            watson_init_hp = stoi(line.substr(15));
        }
        if(line.substr(0,15) == "WATSON_INIT_EXP"){
            watson_init_exp = stoi(line.substr(16));
        }
        if(line.substr(0,9) == "NUM_STEPS"){
            num_steps = stoi(line.substr(10));
        }
        if(line.substr(0,18) == "WATSON_MOVING_RULE"){
            watson_moving_rule = line.substr(19);
        }
        if(line.substr(0,20) == "SHERLOCK_MOVING_RULE"){
            sherlock_moving_rule = line.substr(21);
        }
        if(line.substr(0,17) == "SHERLOCK_INIT_POS"){
            size_t start = line.find('(') + 1;
            size_t end = line.find(',');
            sherlock_init_pos.setRow(stoi(line.substr(start, end - start)));

            start = end + 1;
            end = line.find(')');
            sherlock_init_pos.setCol(stoi(line.substr(start, end - start)));
        }
        if(line.substr(0,15) == "WATSON_INIT_POS"){
            size_t start = line.find('(') + 1;
            size_t end = line.find(',');
            watson_init_pos.setRow(stoi(line.substr(start, end - start)));

            start = end + 1;
            end = line.find(')');
            watson_init_pos.setCol(stoi(line.substr(start, end - start)));
        }
        // cout << "975" << endl;
        if(line.substr(0,17) == "CRIMINAL_INIT_POS"){
            size_t start = line.find('(') + 1;
            size_t end = line.find(',');
            criminal_init_pos.setRow(stoi(line.substr(start, end - start)));

            start = end + 1;
            end = line.find(')');
            criminal_init_pos.setCol(stoi(line.substr(start, end - start)));
        }
        //walls

        if (line.substr(0,11) == "ARRAY_WALLS") {
            string pairs = line.substr(13, line.size() - 14); // extract the list of pairs
            num_walls = 0; // reset num_walls
            if (pairs[0] != ']') { // check if the array is not empty
                for (char c : pairs) {
                    if (c == '(') {
                        num_walls++;
                    }
                }
                if (num_walls > 0) { // only increment num_walls if there are semicolons in the pairs string
                    arr_walls = new Position[num_walls]; // allocate memory for arr_walls

                    stringstream ss(pairs);
                    string pair;
                    int i = 0;

                    while (getline(ss, pair, ';')) { // extract each pair
                        size_t start = pair.find('(') + 1;
                        size_t end = pair.find(',');
                        arr_walls[i].setRow(stoi(pair.substr(start, end - start)));

                        start = end + 1;
                        end = pair.find(')');
                        arr_walls[i].setCol(stoi(pair.substr(start, end - start)));
                        i++;
                    }
                } else {
                    arr_walls = nullptr; // set arr_walls to nullptr if there are no semicolons in the pairs string
                }
            } else {
                arr_walls = nullptr; // set arr_walls to nullptr if the array is empty
            }
        }
        //fake walls
        if (line.substr(0,16) == "ARRAY_FAKE_WALLS") {
            string pairs = line.substr(18, line.size() - 19); // extract the list of pairs
            num_fake_walls = 0; // reset num_fake_walls
            if (pairs[0] != ']') { // check if the array is not empty
                for (char c : pairs) {
                    if (c == '(') {
                        num_fake_walls++;
                    }
                }
                if (num_fake_walls > 0) { // only increment num_fake_walls if there are semicolons in the pairs string
                    arr_fake_walls = new Position[num_fake_walls]; // allocate memory for arr_fake_walls

                    stringstream ss(pairs);
                    string pair;
                    int i = 0;

                    while (getline(ss, pair, ';')) { // extract each pair
                        size_t start = pair.find('(') + 1;
                        size_t end = pair.find(',');
                        arr_fake_walls[i].setRow(stoi(pair.substr(start, end - start)));

                        start = end + 1;
                        end = pair.find(')');
                        arr_fake_walls[i].setCol(stoi(pair.substr(start, end - start)));
                        i++;
                    }
                } else {
                    arr_fake_walls = nullptr; // set arr_fake_walls to nullptr if there are no semicolons in the pairs string
                }
            } else {
                arr_fake_walls = nullptr; // set arr_fake_walls to nullptr if the array is empty
            }
        }
    }
}

Configuration::~Configuration()
{
    // TODO: destructor
    delete[] arr_walls;
    arr_walls = nullptr;
    delete[] arr_fake_walls;
    arr_fake_walls = nullptr;
}
string Configuration::str() const
{
    // TODO: trả về chuỗi mô tả file
    string result = "Configuration[\n";
    result += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    result += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    result += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    result += "NUM_WALLS=" + to_string(num_walls) + "\n";
    result += "ARRAY_WALLS=[";
    if (arr_walls != nullptr && num_walls > 0) {
        for (int i = 0; i < num_walls; i++) {
            result += arr_walls[i].str();
            if (i < num_walls - 1) {
                result += ";";
            }
        }
    }
    result += "]\n";
    result += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    result += "ARRAY_FAKE_WALLS=[";
    if (arr_fake_walls != nullptr && num_fake_walls > 0) {
        for (int i = 0; i < num_fake_walls; i++) {
            result += arr_fake_walls[i].str();
            if (i < num_fake_walls - 1) {
                result += ";";
            }
        }
    }
    result += "]\n";
    result += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    result += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
    result += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    result += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    result += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    result += "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
    result += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    result += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    result += "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
    result += "NUM_STEPS=" + to_string(num_steps) + "\n";
    result += "]";

    return result;
}
/*
 * CLASS: StudyPinkProgram
 */
ItemType MagicBook::getType() const
{
    // TODO: get (1 dòng)
    return ItemType::MAGIC_BOOK;
}
string MagicBook::str() const
{
    // TODO: trả về chuỗi biểu diễn (1 dòng)
    return "MagicBook";
}
bool MagicBook::canUse(Character *obj, Robot *robot)
{
    // TODO: điều kiện sử dụng
    // *Sau khi đấm Robot, EXP
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    if(obj->getObjectType() == MovingObjectType::SHERLOCK){
        if(sherlock->getEXP() <= 350 && robot == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    if(obj->getObjectType() == MovingObjectType::WATSON){
        if(watson->getEXP() <= 350 && robot == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}
void MagicBook::use(Character *obj, Robot *robot)
{
    // TODO: tăng EXP
    if(canUse(obj, robot)){
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        Watson *watson = dynamic_cast<Watson *>(obj);
        if(sherlock){
            sherlock->setEXP(ceil(sherlock->getEXP() * 1.25));
        }
        if(watson){
            watson->setEXP(ceil(watson->getEXP() * 1.25));
        }
    }
}
// *CLASS: EnergyDrink
ItemType EnergyDrink::getType() const
{
    // TODO: get (1 dòng)
    return ItemType::ENERGY_DRINK;
}
string EnergyDrink::str() const
{
    // TODO: trả về chuỗi biển diễn (1 dòng)
    return "EnergyDrink";
}
bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    // TODO: điều kiện sử dụng
    // *Sau khi đấm Robot, HP
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    if(obj->getObjectType() == MovingObjectType::SHERLOCK){
        if(sherlock->getHP() <= 100 && robot == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    if(obj->getObjectType() == MovingObjectType::WATSON){
        if(watson->getHP() <= 100 && robot == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}
void EnergyDrink::use(Character *obj, Robot *robot)
{
    // TODO: tăng HP
    if(canUse(obj, robot)){
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        Watson *watson = dynamic_cast<Watson *>(obj);
        if(obj->getObjectType() == MovingObjectType::SHERLOCK){
            sherlock->setHP(ceil(sherlock->getHP() * 1.20));
        }
        if(obj->getObjectType() == MovingObjectType::WATSON){
            watson->setHP(ceil(watson->getHP() * 1.20));
        }
    }
}
// *CLASS: FirstAid
ItemType FirstAid::getType() const
{
    // TODO: get (1 dòng)
    return ItemType::FIRST_AID;
}
string FirstAid::str() const
{
    // TODO: trả về chuỗi biểu diễn (1 dòng)
    return "FirstAid";
}
bool FirstAid::canUse(Character *obj, Robot *robot)
{
    // TODO: điều kiện sử dụng
    // *Sau khi đấm Robot, EXP || HP
    // TODO: điều kiện sử dụng
    // *Sau khi đấm Robot, HP
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    Watson *watson = dynamic_cast<Watson *>(obj);
    if(sherlock){
        if((sherlock->getHP() <= 100 || sherlock->getEXP() <= 350 ) && (robot == nullptr)){
            return true;
        }
        else{
            return false;
        }
    }
    if(watson){
        if((watson->getHP() <= 100 || watson->getEXP() <= 350 ) && (robot == nullptr)){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}
void FirstAid::use(Character *obj, Robot *robot)
{
    // TODO: tăng HP
    if(canUse(obj, robot)){
        Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
        Watson *watson = dynamic_cast<Watson *>(obj);
        if(sherlock){
            sherlock->setHP(ceil(sherlock->getHP() * 1.50));
        }
        if(watson){
            watson->setHP(ceil(watson->getHP() * 1.50));
        }
    }
}
// *CLASS: ExcemptionCard
ItemType ExcemptionCard::getType() const
{
    // TODO: get (1 dòng)
    return ItemType::EXCEMPTION_CARD;
}
string ExcemptionCard::str() const
{
    // TODO: trả về chuỗi biểu diễn (1 dòng)
    return "ExcemptionCard";
}
bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    // TODO: điều kiện sử dụng
    // *Trước khi đấm Robot, Sherlock, HP lẻ
    Sherlock *sherlock = dynamic_cast<Sherlock *>(obj);
    if(obj->getObjectType() == MovingObjectType::SHERLOCK){
        if(sherlock->getHP() % 2 != 0 && robot != nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}
void ExcemptionCard::use(Character *obj, Robot *robot)
{
    // TODO: sinh viên hiện thực theo tư duy code của mình (hàm có thể rỗng)

}
// *CLASS: PassingCard
PassingCard::PassingCard(int i, int j)
{
    // TODO: constructor gán giá trị cho thuộc tính challenge (i,j là tọa độ)
    int t = (11 * i + j) % 4;
    if(t == 0){
        this->challenge = "RobotS";
    }
    if(t == 1){
        this->challenge = "RobotC";
    }
    if(t == 2){
        this->challenge = "RobotSW";
    }
    if(t == 3){
        this->challenge = "all";
    }
}
PassingCard::PassingCard(string chalenge){
    this->challenge = challenge;
}
ItemType PassingCard::getType() const
{
    // TODO: get (1 dòng)
    return ItemType::PASSING_CARD;
}
string PassingCard::str() const
{
    // TODO: trả về chuỗi biểu diễn (1 dòng)
    return "PassingCard";
}
bool PassingCard::canUse(Character *obj, Robot *robot)
{
    // TODO: điều kiện sử dụng
    // *Trước khi đấm Robot, Watson, HP chẵn
    Watson *watson = dynamic_cast<Watson *>(obj);
    if(obj->getObjectType() == MovingObjectType::WATSON){
        if(watson->getHP() % 2 == 0 && robot != nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}
void PassingCard::use(Character *obj, Robot *robot)
{
    // TODO: tương tự ExcemptionCard khi thuộc tính challenge thích hợp, ngược lại sẽ trừ EXP
    Watson *watson = dynamic_cast<Watson *>(obj);
    if(canUse(obj, robot)){
        if(!(challenge == "all")){
            if(!(challenge == "RobotC" && robot->getType() == RobotType::C)){
                if(!(challenge == "RobotS" && robot->getType() == RobotType::S)){
                    if(!(challenge == "RobotSW" && robot->getType() == RobotType::SW)){
                        watson->setEXP(ceil(watson->getEXP() - 50));
                    }
                }
            }
        }
    }
}
// *CLASS: BaseBag
BaseBag::BaseBag(int capacity)
{
    // TODO: constructor gán giá trị cho CÁC THUỘC TÍNH
    this->capacity = capacity;
    this->size = 0;
    head = nullptr;
    // cout << "BaseBag" << endl;
}
BaseBag::~BaseBag()
{
    // TODO: destructor xóa các Node (Lưu ý phải xóa cả item trong Node đó)
    Node *temp = head;
    while(temp != nullptr){
        Node *del = temp;
        temp = temp->next;          //check xem co delete item khong
        delete del;
    }
    head = nullptr;
}

bool BaseBag::insert(BaseItem *item)
{
    // TODO: thêm Node chứa item vào đầu Linked List
    // *Nếu Linked List đầy hoặc item == nullptr thì trả về false
    if(size >= capacity || item == nullptr){
        return false;
    }
    if(head == nullptr){
        head = new Node(item);
        size = 1;
        return true;
    }
    Node *newNode = new Node(item);
    newNode->next = head;
    head = newNode;
    size++;
    return true;
}
BaseItem *BaseBag::get(ItemType itemType)
{
    // TODO: Tìm và trả về item tương ứng với itemType
    // *Đảo vị trí trong BaseBag (2 cách phổ biến: swap Node hoặc con trỏ item của Node)
    // *Sau đó xóa Node (lưu ý cập nhật con trỏ head)
    
    if(head == nullptr){
        return nullptr;
    }
    if(head->item->getType() == itemType){
        Node *temp = head;
        head = head->next;
        BaseItem *item = temp->item;
        delete temp;
        size--;
        return item;
    }
    Node *prev = head;
    Node *curr = head->next;
    while(curr!=nullptr){
        if(curr->item->getType() == itemType){
            // Swap curr with head
            BaseItem *tempItem = head->item;
            head->item = curr->item;
            curr->item = tempItem;

            // Delete the found node (which is now the head)
            Node *temp = head;
            head = head->next;
            BaseItem *item = temp->item;
            delete temp;
            size--;
            return item;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}
BaseItem *BaseBag::get(){
    return nullptr;
}
string BaseBag::str() const
{
    // TODO: trả về chuỗi biểu diễn
    // *"Bag[count=<thuộc tính size>;<danh sách tên các item cách nhau bởi dấu ,>]"
    stringstream ss;
    ss << "Bag[count=" << size << ";";
    Node * curr = head;
    while(curr != nullptr){
        ss << curr->item->str();
        curr = curr->next;
        if(curr != nullptr){
            ss << ",";
        }
        // curr = curr->next;
    }
    ss << "]";
    return ss.str();
}
bool BaseBag::checkItem(ItemType itemType){
    Node *curr = head;
    while(curr != nullptr){
        if(curr->item->getType() == itemType){
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// *CLASS: SherlockBag
SherlockBag::SherlockBag(Sherlock *character)
    : BaseBag(13), sherlock(character)
{
    // TODO: constructor class con
    this->head = nullptr;
    this->size = 0; 
    this->capacity = 13;
    this->obj = character; //
}
BaseItem *SherlockBag::get()
{
    if(head == nullptr){
        return nullptr;
    }
    if(head->item->canUse(sherlock, nullptr)){
        Node *temp = head;
        head = head->next;
        BaseItem *item = temp->item;
        delete temp;
        size--;
        return item;
    }
    Node *prev = head;
    Node *curr = head->next;
    while(curr!=nullptr){
        if(curr->item->canUse(sherlock, nullptr)){
            // Swap curr with head
            BaseItem *tempItem = head->item;
            head->item = curr->item;
            curr->item = tempItem;

            // Delete the found node (which is now the head)
            Node *temp = head;
            head = head->next;
            BaseItem *item = temp->item;
            delete temp;
            size--;
            return item;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}
WatsonBag::WatsonBag(Watson *character)
    : BaseBag(15), watson(character)

{
    // TODO: constructor class con
    this->head = nullptr;
    this->size = 0;
    this->capacity = 15;
    this->obj = character;
}
BaseItem *WatsonBag::get()
{
    // TODO: tìm và trả về item WATSON CÓ THỂ SỬ DỤNG gấn đầu túi nhất, đảo lên đầu và xóa khỏi Linked List (cập nhật head)
    if(head == nullptr){
        return nullptr;
    }
    if(head->item->canUse(watson, nullptr)){
        Node *temp = head;
        head = head->next;
        BaseItem *item = temp->item;
        delete temp;
        size--;
        return item;
    }
    Node *prev = head;
    Node *curr = head->next;
    while(curr!=nullptr){
        if(curr->item->canUse(watson, nullptr)){
            // Swap curr with head
            BaseItem *tempItem = head->item;
            head->item = curr->item;
            curr->item = tempItem;

            // Delete the found node (which is now the head)
            Node *temp = head;
            head = head->next;
            BaseItem *item = temp->item;
            delete temp;
            size--;
            return item;
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}
Robot::Robot(int index, const Position &pos, Map *map, Criminal *criminal, const string &name)
    : MovingObject(index, pos, map, name), criminal(criminal) //! khác
{
    // TODO: tính toán loại item
    int p = pos.getRow() * pos.getCol();
    int s = p;
    while(s >= 10){
        int sum = 0;
        while(p > 0){
            sum += p % 10;
            p /= 10;
        }
        s = sum;
        p = sum;
    }
    // cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<s<<endl;
    if(s >= 0 && s <= 1){
        item = new MagicBook();
    }
    else if(s >= 2 && s <= 3){
        item = new EnergyDrink();
    }
    else if(s >= 4 && s <= 5){
        item = new FirstAid();
    }
    else if(s >= 6 && s <= 7){
        item = new ExcemptionCard();
    }
    else if(s >= 8 && s <= 9){
        item = new PassingCard(pos.getRow(), pos.getCol()); 
    }
    
}
Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), hp(init_hp), exp(init_exp), index_moving_rule(0)
{
    // TODO: thêm thuộc tính bag
    this->bag = new SherlockBag(this);
}
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), hp(init_hp), exp(init_exp), index_moving_rule(0)
{
    // TODO: thêm thuộc tính bag
    this->bag = new WatsonBag(this);
}
// *------------------------------------------------------------------
// *
// *------------------------------------------------------------------
// !-----------------------------------
// ! Lưu ý về việc xét thông số khi tăng giảm
// ! Các thay đổi thông số là dựa trên thông số hiện tại
// ! Các thông số không phải số nguyên THỰC HIỆN LÀM TRÒN LÊN -> NHÂN VẬT CHỈ HI SINH KHI INIT HP = 0
// !-----------------------------------
// *CLASS: ArrayMovingObject
bool ArrayMovingObject::checkMeet(int index) const
{
    
    // TODO: Xét va chạm của nhân vật (theo index) với các nhân vật khác trong array
    // TODO: Thực hiện xử lý các sự kiện xảy ra (thử thách, thêm item, bắt Criminal)
    MovingObject *currentObj = this->get(index);
    for(int i = 0; i < this->size(); i++){
        if(i != index){
            MovingObject *otherObj = this->get(i);
            if(currentObj->getObjectType() == SHERLOCK){
                if(currentObj->getCurrentPosition().isEqual(otherObj->getCurrentPosition())){
                    if(otherObj->getObjectType() == CRIMINAL){
                        return true;
                    }
                    else if(otherObj->getObjectType() == ROBOT){
                        Sherlock *sherlock = dynamic_cast<Sherlock *>(currentObj);
                        Robot* robot = dynamic_cast<Robot *>(otherObj);
                        if(robot->getType() == C && sherlock->meet(dynamic_cast<RobotC *>(robot)) == true){
                            Position criminalPos = robot->getNextPosition();
                            sherlock->setPos(criminalPos);
                            return true;
                        }
                        else if(robot->getType() == S){
                            sherlock->meet(dynamic_cast<RobotS *>(robot));
                        }
                        else if(robot->getType() == SW){
                            sherlock->meet(dynamic_cast<RobotSW *>(robot));
                        }
                        else if(robot->getType() == W){
                            sherlock->meet(dynamic_cast<RobotW *>(robot));
                        }
                    }
                    else if(otherObj->getObjectType() == WATSON){
                        Watson *watson = dynamic_cast<Watson *>(otherObj);
                        Sherlock *sherlock = dynamic_cast<Sherlock *>(currentObj);
                        sherlock->meet(watson);
                    }
                }
            }
            else if(currentObj->getObjectType() == WATSON){
                if(currentObj->getCurrentPosition().isEqual(otherObj->getCurrentPosition())){
                    if(otherObj->getObjectType() == CRIMINAL){
                        return true;
                    }
                    else if(otherObj->getObjectType() == ROBOT){
                        Watson *watson = dynamic_cast<Watson *>(currentObj);
                        Robot* robot = dynamic_cast<Robot *>(otherObj);
                        if(robot->getType() == C){
                            watson->meet(dynamic_cast<RobotC *>(robot));
                        }
                        if(robot->getType() == S){
                            watson->meet(dynamic_cast<RobotS *>(robot));
                        }
                        if(robot->getType() == SW){
                            watson->meet(dynamic_cast<RobotSW *>(robot));
                        }
                        if(robot->getType() == W){
                            watson->meet(dynamic_cast<RobotW *>(robot));
                        }
                    }
                    else if(otherObj->getObjectType() == SHERLOCK){
                        Sherlock *sherlock = dynamic_cast<Sherlock *>(otherObj);
                        Watson *watson = dynamic_cast<Watson *>(currentObj);
                        watson->meet(sherlock);
                    }
                }
            }
            else if(currentObj->getObjectType() == CRIMINAL){
                if(currentObj->getCurrentPosition().isEqual(otherObj->getCurrentPosition())){
                    if(otherObj->getObjectType() == SHERLOCK){
                        return true;
                    }
                    else if(otherObj->getObjectType() == WATSON){
                        return true;
                    }
                }
            }
            else if(currentObj->getObjectType() == ROBOT){
                if(currentObj->getCurrentPosition().isEqual(otherObj->getCurrentPosition())){
                    Robot *robot = dynamic_cast<Robot *>(currentObj);
                    if(otherObj->getObjectType() == SHERLOCK){
                        Sherlock *sherlock = dynamic_cast<Sherlock *>(otherObj);
                        if(robot->getType() == C && sherlock->meet(dynamic_cast<RobotC *>(robot)) == true){
                            return true;
                        }
                        if(robot->getType() == S){
                            sherlock->meet(dynamic_cast<RobotS *>(robot));
                        }
                        if(robot->getType() == SW){
                            sherlock->meet(dynamic_cast<RobotSW *>(robot));
                        }
                        if(robot->getType() == W){
                            sherlock->meet(dynamic_cast<RobotW *>(robot));
                        }
                    }
                    if(otherObj->getObjectType() == WATSON){
                        Watson *watson = dynamic_cast<Watson *>(otherObj);
                        if(robot->getType() == C){
                            watson->meet(dynamic_cast<RobotC *>(robot));
                        }
                        if(robot->getType() == S){
                            watson->meet(dynamic_cast<RobotS *>(robot));
                        }
                        if(robot->getType() == SW){
                            watson->meet(dynamic_cast<RobotSW *>(robot));
                        }
                        if(robot->getType() == W){
                            watson->meet(dynamic_cast<RobotW *>(robot));
                        }
                    }
                }
            }   
        }
    }
    return false;
}
// *CLASS: Sherlock
// ! Lưu ý: ExcemptionCard được dùng để BỎ QUA nếu THỬ THÁCH THẤT BẠI -> xem như không thực hiện thử thách -> không gọi get lần 2
// ! Thực hiện get từ bag sau khi insert item

// *CLASS: Sherlock
// ! Lưu ý: ExcemptionCard được dùng để BỎ QUA nếu THỬ THÁCH THẤT BẠI -> xem như không thực hiện thử thách -> không gọi get lần 2
// ! Thực hiện get từ bag sau khi insert item
void Sherlock::setPos(Position pos)
{
    // TODO: Sử dụng trong trường hợp thắng RobotC sẽ dịch chuyển đến vị trí Criminal
    this->pos = pos;
}
bool Sherlock::meet(RobotS *robotS)
{
    // TODO: Xử lý khi gặp robot S
    bool win = false;
    if(this->getHP() % 2 == 1){             //check dieu kien sdung excemption card
        BaseItem *item = this->getBag()->get(EXCEMPTION_CARD);
    }
    if(this->getEXP() > 400){
        win = true;
        BaseItem *rbItem = robotS->NewItem();
        this->bag->insert(rbItem);
    }       // 
    else{
        this->setEXP(ceil(this->getEXP() * 0.90));
        win = false;
    }
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotS);                      //co xai hay ko, test case 14
    }
    return win;
}
bool Sherlock::meet(RobotW *robotW)
{
    // TODO: Xử lý khi gặp robot W
    bool win = false;
    if(this->getHP() % 2 == 1){             //check dieu kien sdung excemption card
        BaseItem *item = this->getBag()->get(EXCEMPTION_CARD);
        if(item != nullptr){        //co excemption card
            item->use(this, robotW);
            win = true;
            BaseItem *rbItem = robotW->NewItem();
            this->bag->insert(rbItem);
        }
    }
    else{
        win = true;
        BaseItem *rbItem = robotW->NewItem();
        this->bag->insert(rbItem);
    }
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotW);
    }
    return win;
}
bool Sherlock::meet(RobotSW *robotSW)
{
    // TODO: Xử lý khi gặp robot SW
    bool win = false;
    if(this->getHP() % 2 == 1){             //check dieu kien sdung excemption card
        BaseItem *item = this->getBag()->get(EXCEMPTION_CARD);
        if(item != nullptr){        //co excemption card    
            if(this->getEXP() > 300 && this->getHP() > 335){
                item->use(this, robotSW);
                win = true;
                BaseItem *rbItem = robotSW->NewItem();
                this->bag->insert(rbItem);
            }        // 
            else{
                win = false;
            }
        }
    }
    else{
        if(this->getEXP() > 300 && this->getHP() > 335){
            win = true;
            BaseItem *rbItem = robotSW->NewItem();
            this->bag->insert(rbItem);
        }
        else{
            this->setEXP(ceil(this->getEXP() * 0.85));
            this->setHP(ceil(this->getHP() * 0.85));
            win = false;
        }
    }
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotSW);
    }
    return win;
}
bool Sherlock::meet(RobotC *robotC)
{
    // TODO: Xử lý khi gặp robot C
    bool win = false;
    if(this->getHP() % 2 == 1){             //check dieu kien sdung excemption card
        BaseItem *item = this->getBag()->get(EXCEMPTION_CARD);
        if(item != nullptr){        //co excemption card    
            if(this->getEXP() > 500){
                item->use(this, robotC);
                win = true;
                this->getCurrentPosition() = robotC->getNextPosition(); //nhay toi vi tri cua robot
            }        // 
            else{
                win = false;
                BaseItem *rbItem = robotC->NewItem();
                this->bag->insert(rbItem);
            }
        }
    }
    else{
        if(this->getEXP() > 500){
            win = true;
            this->getCurrentPosition() = robotC->getNextPosition(); //nhay toi vi tri cua robot
        }
        else{
            win = false;
            BaseItem *rbItem = robotC->NewItem();
            this->bag->insert(rbItem);
        }
    }

    
    
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotC);
    }
    return win;
}
bool Sherlock::meet(Watson *watson)
{
    // TODO: Xử lý trao đổi thẻ ExcemptionCard
    // TODO: KHI CẢ 2 ĐỀU CÓ THỂ TRAO ĐỔI && ĐỔI TOÀN BỘ NẾU NHIỀU HƠN 1 (KỂ CẢ KHI ĐỐI PHƯƠNG)
    bool exchange = false;
    if(watson->getBag()->checkItem(ItemType::EXCEMPTION_CARD) == false || this->getBag()->checkItem(ItemType::PASSING_CARD) == false){
        return false;
    }
    else{
        while(this->getBag()->checkItem(ItemType::EXCEMPTION_CARD)){
            BaseItem *item = this->getBag()->get(ItemType::EXCEMPTION_CARD);
            watson->getBag()->insert(item);
            exchange = true;
        }
        while(watson->getBag()->checkItem(ItemType::PASSING_CARD)){
            BaseItem *item = watson->getBag()->get(ItemType::PASSING_CARD);
            this->getBag()->insert(item);
            exchange = true;
        }
    }
    return exchange;
}
// *CLASS: Watson
// ! Lưu ý: PassingCard được dùng khi GẶP ROBOT để bỏ qua thử thách nếu đúng loại thẻ
// ! Thực hiện get từ bag sau khi insert item
bool Watson::meet(RobotS *robotS)
{
    // TODO: Xử lý trao đổi khi gặp robot S
    bool win = false;
    if(this->getHP() == 0){
        BaseItem *item = this->getBag()->get(PASSING_CARD);
        if(item == new PassingCard("RobotS") && item != nullptr){  
            item->use(this, robotS);       //handle truong hop null va khac challenge
            win = true;
        }           
    }
    else{
        win = true;
    }
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotS);
    }
    return win;
}
bool Watson::meet(RobotW *robotW)
{
    // TODO: Xử lý trao đổi khi gặp robot W
 
    bool win = false;
    BaseItem *item = nullptr;
    if(this->getHP() % 2 == 0){
        item = this->getBag()->get(PASSING_CARD);
        if(item != nullptr){
            item->use(this, robotW);
        }
    }
    
    if(this->getHP() > 350 || item != nullptr){
        win = true;
    }
    else{
        this->setHP(ceil(this->getHP() * 0.95));
        win = false;
    }
   
    if(win == true){
        
        BaseItem *rbItem = robotW->NewItem();
        this->bag->insert(rbItem);
    }
 
    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, nullptr);
   
    }
    return win;
}
bool Watson::meet(RobotSW *robotSW)
{
    // TODO: Xử lý trao đổi khi gặp robot SW
    bool win = false;
    if(this->getHP() == 0){
        BaseItem *item = this->getBag()->get(PASSING_CARD);
        if(item == new PassingCard("RobotSW") && item != nullptr){   
            item->use(this, robotSW);        //handle truong hop null va khac challenge
            win = true;
            BaseItem *rbItem = robotSW->NewItem();
            this->bag->insert(rbItem);
        }
    }
    else{
        if(this->getEXP() > 600 && this->getHP() > 165){
            win = true;
            BaseItem *rbItem = robotSW->NewItem();
            this->bag->insert(rbItem);
        }
        else{
            this->setEXP(ceil(this->getEXP() * 0.85));
            this->setHP(ceil(this->getHP() * 0.85));
            win = false;
        }
    }

    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotSW);
    }
    return win;
}
bool Watson::meet(RobotC *robotC)
{
    // TODO: Xử lý trao đổi khi gặp robot C
    bool win = false;
    if(this->getHP() == 0){
        BaseItem *item = this->getBag()->get(PASSING_CARD);
        if(item == new PassingCard("RobotC") && item != nullptr){   
            item->use(this, robotC);        //handle truong hop null va khac challenge
            win = true;
        }
    }
    else{
        win = true;
    }
    BaseItem *rbItem = robotC->NewItem();
    this->bag->insert(rbItem); 

    BaseItem *usableItem = this->getBag()->get();
    if(usableItem != nullptr){
        usableItem->use(this, robotC);
    }
    return win;
}
bool Watson::meet(Sherlock *sherlock)
{
    // TODO: Xử lý trao đổi khi gặp Sherlock
    bool exchange = false;
    if(sherlock->getBag()->checkItem(ItemType::PASSING_CARD) == false || this->getBag()->checkItem(ItemType::EXCEMPTION_CARD) == false){
        return false;
    }
    else{
        while(sherlock->getBag()->checkItem(ItemType::PASSING_CARD)){
            BaseItem *item = sherlock->getBag()->get(ItemType::PASSING_CARD);
            this->getBag()->insert(item);
            exchange = true;
        }
        while(this->getBag()->checkItem(ItemType::EXCEMPTION_CARD)){
            BaseItem *item = this->getBag()->get(ItemType::EXCEMPTION_CARD);
            sherlock->getBag()->insert(item);
            exchange = true;
        }
    }
    
    return exchange;
}

BaseBag *Sherlock::getBag() const
{
    // TODO: get bag
    return bag;
}
BaseBag *Watson::getBag() const
{
    // TODO: get bag
    return bag;
}
void StudyPinkProgram::run(bool verbose){
    if (isStop() == false){
        
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
                arr_mv_objs->get(i)->move();
               
                 //! check meet
                arr_mv_objs->checkMeet(i);
                if (isStop())
                {
                    printStep(istep);
                    break;
                }
              //! điều kiện thêm robot
                if(arr_mv_objs->get(i)->getObjectType() == CRIMINAL){
                    Criminal *criminal = dynamic_cast<Criminal *>(criminal);
                    if(criminal->getCount() % 3 == 0){
                        if(criminal->getCount() > 1 && arr_mv_objs->isFull() == false){
                            arr_mv_objs->add(Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson));
                        }
                    }
                }
                if (verbose)
                {
                
                    printStep(istep);
                }
            }
            if (isStop())
                break;
        }
    }
    printResult();
    }
void StudyPinkProgram::run(bool verbose, ofstream &OUTPUT)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
   
    if (isStop() == false)
    {
     
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
         
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
              
                arr_mv_objs->get(i)->move();
               
                 //! check meet
             
                arr_mv_objs->checkMeet(i);
             
                if (isStop())
                {
                    printInfo(istep, i, OUTPUT); //! ở run ko ouput bỏ cái này 
                    printStep(istep);
                    break;
                }
              //! điều kiện thêm robot
                if(arr_mv_objs->get(i)->getObjectType() == CRIMINAL){
                      
                    Criminal *criminal = dynamic_cast<Criminal *>(arr_mv_objs->get(i));
                    
                    if(criminal->getCount() % 3 == 0){
                         
                        if(criminal->getCount() > 1 && arr_mv_objs->isFull() == false){
                        
                            arr_mv_objs->add(Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson));
                        }
                    }  
                    
                }
                if (verbose)
                {
                    printInfo(istep, i, OUTPUT); //! ở run ko ouput bỏ cái này 
                    printStep(istep);
                }
            }
            if (isStop())
                break;
        }
    }

    printResult();
}