#include "study_in_pink2.h"

/*
 * /---3.1---Class::MapElement/
 */

MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    return this->type;
}

Path::Path() : MapElement(ElementType::PATH) {}

Wall::Wall() : MapElement(ElementType::WALL) {}

FakeWall::FakeWall(int in_req_exp) : MapElement(ElementType::FAKE_WALL)
{
    this->req_exp = in_req_exp;
}

int FakeWall::getReqExp() const
{
    return this->req_exp;
}

/*
 * /---3.2---Class::Map/
 */

Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
{
    this->map = nullptr;
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    if (this->num_cols == 0 || this->num_rows == 0)
    {
        return;
    }
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; ++i)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; ++j)
        {
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_fake_walls; ++i)
    {
        int row = array_fake_walls[i].getRow();
        int col = array_fake_walls[i].getCol();
        if (row < num_rows && num_cols > col && row >= 0 && col >= 0)
        {
            int in_req_exp = (257 * row + col * 139 + 89) % 900 + 1;
            delete map[row][col];
            map[row][col] = new FakeWall(in_req_exp);
        }
    }

    for (int i = 0; i < num_walls; ++i)
    {
        int row = array_walls[i].getRow();
        int col = array_walls[i].getCol();
        if (row < num_rows && num_cols > col && row >= 0 && col >= 0)
        {
            delete map[row][col];
            map[row][col] = new Wall();
        }
    }
}

Map::~Map()
{
    if (this->num_cols == 0 || this->num_rows == 0)
    {
        return;
    }
    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int row = pos.getRow();
    int col = pos.getCol();
    int row_obj = mv_obj->getCurrentPosition().getRow();
    int col_obj = mv_obj->getCurrentPosition().getCol();
    if (row >= 0 && row < num_rows && col >= 0 && col < num_cols && row_obj >= 0 && row_obj < num_rows && col_obj >= 0 && col_obj < num_cols)
    {
        if (mv_obj->getCharacterType() != SHERLOCK && mv_obj->getCharacterType() != ROBOT && mv_obj->getCharacterType() != WATSON && mv_obj->getCharacterType() != CRIMINAL)
        {
            return false;
        }
        else
        {
            if (map[row][col]->getType() == ElementType::PATH)
            {
                return true;
            }
            else if (map[row][col]->getType() == ElementType::WALL)
            {
                return false;
            }
            else if (map[row][col]->getType() == ElementType::FAKE_WALL)
            {
                if (mv_obj->getCharacterType() != MovingCharacter::WATSON)
                {
                    return true;
                }
                else
                {
                    if (((Watson *)mv_obj)->getEXP() > ((FakeWall *)map[row][col])->getReqExp())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}

/*
 * /---3.3---Class::Position/
 */

Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    string s, s1;
    int a;
    s = str_pos.substr(1, str_pos.size() - 2);
    s1 = s.substr(0, s.find(","));
    stringstream ss(s1);
    ss >> a;
    this->r = a;
    s1 = s.substr(s.find(",") + 1);
    stringstream ss1(s1);
    ss1 >> a;
    this->c = a;
}

int Position::getRow() const
{
    return this->r;
}

int Position::getCol() const
{
    return this->c;
}

void Position::setRow(int r)
{
    this->r = r;
}

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{
    return "(" + to_string(this->r) + "," + to_string(this->c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{
    if (this->r == in_r && this->c == in_c)
    {
        return true;
    }
    return false;
}

bool Position::isEqual(Position position) const
{
    if (this->r == position.getRow() && this->c == position.getCol())
    {
        return true;
    }
    return false;
}

const Position Position::npos = Position(-1, -1);

/*
 * /---3.4--Class::MovingObject/
 */

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->map = nullptr;
    this->index = index;
    this->pos = pos;
    if (map != nullptr)
    {
        this->map = map;
    }
    this->name = name;
}

Position MovingObject::getCurrentPosition() const
{
    return this->pos;
}

MovingObject::~MovingObject()
{
    //  delete this->map;
}

/*
 * /---3.5---Class::Character&Sherlock/
 */

Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(index, pos, map, name)
{
    this->index = index;
    this->map = map;
    this->pos = pos;
    this->name = name;
}

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock")
{
    this->index = index;
    this->moving_rule = moving_rule;
    this->pos = init_pos;
    this->map = map;
    this->hp = init_hp;
    this->exp = init_exp;
    this->name = "Sherlock";
    this->index_moving_rule = 0;
    this->bag = new SherlockBag(this);
    this->moving_rule_str = moving_rule;
}

Sherlock::~Sherlock()
{
    delete bag;
}

Position Sherlock::getNextPosition()
{
    Position pos1 = this->pos;
    if (this->moving_rule[0] == 'U')
    {
        pos1.setRow(this->getCurrentPosition().getRow() - 1);
        if (this->map->isValid(pos1, ((MovingObject *)this)))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'D')
    {
        pos1.setRow(getCurrentPosition().getRow() + 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'L')
    {
        pos1.setCol(getCurrentPosition().getCol() - 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'R')
    {
        pos1.setCol(getCurrentPosition().getCol() + 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    return Position::npos;
}

void Sherlock::move()
{
    if (this->exp > 0)
    {
        if (this->getNextPosition().getCol() != Position::npos.getCol() && this->getNextPosition().getRow() != Position::npos.getRow())
        {
            this->pos = this->getNextPosition();
        }
        this->moving_rule.push_back(this->moving_rule[0]);
        this->moving_rule.erase(0, 1);
    }
}

string Sherlock::str() const
{
    return "Sherlock[index=" + to_string(this->Sherlock::index) + ";pos=" + this->Sherlock::pos.str() + ";moving_rule=" + this->Sherlock::moving_rule_str + "]";
}

MovingCharacter Sherlock::getCharacterType() const
{
    return MovingCharacter::SHERLOCK;
}

int Sherlock::getHP() const
{
    return this->hp;
}

int Sherlock::getEXP() const
{
    return this->exp;
}

BaseBag *Sherlock::getBag() const
{
    return this->bag;
}

void Sherlock::setHP(int hp)
{
    if (hp < 0)
    {
        this->hp = 0;
    }
    else if (hp > 500)
    {
        this->hp = 500;
    }
    else
    {
        this->hp = hp;
    }
}

void Sherlock::setEXP(int exp)
{
    if (exp < 0)
    {
        this->exp = 0;
    }
    else if (exp > 999)
    {
        this->exp = 999;
    }
    else
    {
        this->exp = exp;
    }
}

void Sherlock::setPos(Position pos)
{
    this->pos = pos;
}

bool Sherlock::event(RobotS *robotS)
{
    BaseItem *item_use = nullptr;
    bool check = false;
    if (this->bag->checkItem(EXCEMPTION_CARD) && this->getHP() % 2 == 1)
    {
        check = true;
        this->bag->get(EXCEMPTION_CARD);
    }

    if (this->getEXP() > 400)
    {
        this->bag->insert(robotS->NewItem());
        item_use = this->getBag()->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return true;
    }
    else
    {
        if (check == false)
        {
            this->setEXP(ceil(this->getEXP() * 0.9));
        }
        item_use = this->getBag()->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return false;
    }
}

bool Sherlock::event(RobotW *robotW)
{
    BaseItem *item_use = nullptr;
    bool check = false;
    if (this->bag->checkItem(EXCEMPTION_CARD) && this->getHP() % 2 == 1)
    {
        check = true;
        this->bag->get(EXCEMPTION_CARD);
    }

    this->bag->insert(robotW->NewItem());
    item_use = this->getBag()->get();
    if (item_use != nullptr)
    {
        item_use->use(this, nullptr);
    }
    return true;
}

bool Sherlock::event(RobotSW *robotSW)
{
    BaseItem *item_use = nullptr;
    bool check = false;
    if (this->bag->checkItem(EXCEMPTION_CARD) && this->getHP() % 2 == 1)
    {
        check = true;
        this->bag->get(EXCEMPTION_CARD);
    }

    if (this->getEXP() > 300 && this->getHP() > 335)
    {
        this->bag->insert(robotSW->NewItem());
        item_use = this->getBag()->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return true;
    }
    else
    {
        if (check == false)
        {
            this->setEXP(ceil(this->getEXP() * 0.85));
            this->setHP(ceil(this->getHP() * 0.85));
        }
        item_use = dynamic_cast<SherlockBag *>(this->bag)->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return false;
    }
}

bool Sherlock::event(RobotC *robotC)
{
    bool check = false;
    if (this->bag->checkItem(EXCEMPTION_CARD) && this->getHP() % 2 == 1)
    {
        check = true;
        this->bag->get(EXCEMPTION_CARD);
    }
    BaseItem *item_use = nullptr;
    if (this->getEXP() > 500)
    {
        item_use = dynamic_cast<SherlockBag *>(this->bag)->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return true;
    }
    else
    {
        this->bag->insert(robotC->NewItem());
        item_use = dynamic_cast<SherlockBag *>(this->bag)->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return false;
    }
}

bool Sherlock::event(Watson *watson)
{

    if (bag->checkItem(PASSING_CARD) && watson->getBag()->checkItem(EXCEMPTION_CARD))
    {
        while (bag->checkItem(PASSING_CARD))
        {
            watson->getBag()->insert(bag->get(PASSING_CARD));
        }
        while (watson->getBag()->checkItem(EXCEMPTION_CARD))
        {
            bag->insert(watson->getBag()->get(EXCEMPTION_CARD));
        }
        return true;
    }
    return false;
}

/*
 * /---3.6---Class::Watson/
 */

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson")
{
    this->index = index;
    this->moving_rule = moving_rule;
    this->pos = init_pos;
    this->map = map;
    this->hp = init_hp;
    this->exp = init_exp;
    this->name = "Watson";
    this->index_moving_rule = 0;
    this->bag = new WatsonBag(this);
    this->moving_rule_str = moving_rule;
}

Watson::~Watson()
{
    delete bag;
}

Position Watson::getNextPosition()
{
    Position pos1 = this->pos;
    if (this->moving_rule[0] == 'U')
    {
        pos1.setRow(this->getCurrentPosition().getRow() - 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'D')
    {
        pos1.setRow(this->getCurrentPosition().getRow() + 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'L')
    {
        pos1.setCol(this->getCurrentPosition().getCol() - 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    else if (this->moving_rule[0] == 'R')
    {
        pos1.setCol(this->getCurrentPosition().getCol() + 1);
        if (this->map->isValid(pos1, (MovingObject *)this))
        {
            return pos1;
        }
        else
        {
            return Position::npos;
        }
    }
    return Position::npos;
}

void Watson::move()
{
    if (this->exp > 0)
    {
        if (this->getNextPosition().getCol() != Position::npos.getCol() && this->getNextPosition().getRow() != Position::npos.getRow())
        {
            this->pos = this->getNextPosition();
        }
        this->moving_rule.push_back(this->moving_rule[0]);
        this->moving_rule.erase(0, 1);
    }
}

string Watson::str() const
{
    return "Watson[index=" + to_string(this->Watson::index) + ";pos=" + this->Watson::pos.str() + ";moving_rule=" + this->Watson::moving_rule_str + "]";
}

MovingCharacter Watson::getCharacterType() const
{
    return MovingCharacter::WATSON;
}

int Watson::getHP() const
{
    return this->hp;
}

int Watson::getEXP() const
{
    return this->exp;
}

BaseBag *Watson::getBag() const
{
    return this->bag;
}

void Watson::setHP(int hp)
{
    if (hp < 0)
    {
        this->hp = 0;
    }
    else if (hp > 500)
    {
        this->hp = 500;
    }
    else
    {
        this->hp = hp;
    }
}

void Watson::setEXP(int exp)
{
    if (exp < 0)
    {
        this->exp = 0;
    }
    else if (exp > 999)
    {
        this->exp = 999;
    }
    else
    {
        this->exp = exp;
    }
}

bool Watson::event(RobotS *robotS)
{
    BaseItem *items = nullptr;
    BaseItem *item_use = nullptr;
    if (this->getBag()->checkItem(PASSING_CARD) && this->getHP() % 2 == 0)
    {
        items = this->bag->get(PASSING_CARD);
    }
    if (items != nullptr)
    {
        items->use(this, robotS);
    }
    item_use = this->getBag()->get();
    if (item_use != nullptr)
    {
        item_use->use(this, nullptr);
    }
    return true;
}

bool Watson::event(RobotW *robotW)
{
    BaseItem *items = nullptr;
    if (this->getBag()->checkItem(PASSING_CARD) && this->getHP() % 2 == 0)
    {
        items = this->bag->get(PASSING_CARD);
    }

    BaseItem *item_use = nullptr;
    if (items != nullptr)
    {
        items->use(this, robotW);
        this->bag->insert(robotW->NewItem());
        item_use = this->getBag()->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return true;
    }
    else
    {
        if (this->getHP() > 350)
        {
            this->bag->insert(robotW->NewItem());
            item_use = this->getBag()->get();
            if (item_use != nullptr)
            {
                item_use->use(this, nullptr);
            }
            return true;
        }
        else
        {
            this->setHP(ceil(this->getHP() * 0.95));
            item_use = this->getBag()->get();
            if (item_use != nullptr)
            {
                item_use->use(this, nullptr);
            }
            return false;
        }
    }
}

bool Watson::event(RobotSW *robotSW)
{
    BaseItem *items = nullptr;
    if (this->getBag()->checkItem(PASSING_CARD) && this->getHP() % 2 == 0)
    {
        items = this->bag->get(PASSING_CARD);
    }
    BaseItem *item_use = nullptr;
    if (items != nullptr)
    {
        items->use(this, robotSW);
        this->bag->insert(robotSW->NewItem());
        item_use = this->getBag()->get();
        if (item_use != nullptr)
        {
            item_use->use(this, nullptr);
        }
        return true;
    }
    else
    {
        if (this->getHP() > 165 && this->getEXP() > 600)
        {
            this->bag->insert(robotSW->NewItem());
            item_use = this->getBag()->get();
            if (item_use != nullptr)
            {
                item_use->use(this, nullptr);
            }
            return true;
        }
        else
        {
            this->setHP(ceil(this->getHP() * 0.85));
            this->setEXP(ceil(this->getEXP() * 0.85));
            item_use = this->getBag()->get();
            if (item_use != nullptr)
            {
                item_use->use(this, nullptr);
            }
            return false;
        }
    }
}

bool Watson::event(RobotC *robotC)
{
    BaseItem *items = nullptr;
    if (this->getBag()->checkItem(PASSING_CARD) && this->getHP() % 2 == 0)
    {
        items = this->bag->get(PASSING_CARD);
    }
    BaseItem *item_use = nullptr;
    if (items != nullptr)
    {
        items->use(this, robotC);
    }
    this->bag->insert(robotC->NewItem());
    item_use = this->getBag()->get();
    if (item_use != nullptr)
    {
        item_use->use(this, nullptr);
    }
    return true;
}

bool Watson::event(Sherlock *sherlock)
{
    if (sherlock->getBag()->checkItem(PASSING_CARD) && this->getBag()->checkItem(EXCEMPTION_CARD))
    {
        while (sherlock->getBag()->checkItem(PASSING_CARD))
        {
            this->getBag()->insert(bag->get(PASSING_CARD));
        }
        while (this->getBag()->checkItem(EXCEMPTION_CARD))
        {
            sherlock->getBag()->insert(sherlock->getBag()->get(EXCEMPTION_CARD));
        }
        return true;
    }
    return false;
}

/*
 * /---3.7---Class::Criminal/
 */

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(index, init_pos, map, "Criminal")
{
    this->sherlock = nullptr;
    this->watson = nullptr;
    this->Criminal::index = index;
    this->Criminal::pos = init_pos;
    this->prev_pos = init_pos;
    this->Criminal::map = map;
    if (sherlock != nullptr)
    {
        this->Criminal::sherlock = sherlock;
    }
    if (watson != nullptr)
    {
        this->Criminal::watson = watson;
    }

    this->Criminal::name = "Criminal";
    this->Criminal::count = 0;
}

Criminal::~Criminal()
{
    // delete sherlock;
    // delete watson;
}

int Criminal::manhattanDistance(const Position &pos1, const Position &pos2) const
{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
}

Position Criminal::getNextPosition()
{
    Position pos1 = this->pos;
    int d_u, d_l, d_d, d_r, maxx;
    pos1.setRow(this->getCurrentPosition().getRow() - 1); // case U
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_u = this->manhattanDistance(pos1, this->sherlock->getCurrentPosition()) + manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_u = -1;
    }
    pos1 = this->pos;
    pos1.setCol(this->getCurrentPosition().getCol() - 1); // case L
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_l = this->manhattanDistance(pos1, this->sherlock->getCurrentPosition()) + this->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_l = -1;
    }
    pos1 = this->pos;
    pos1.setRow(this->getCurrentPosition().getRow() + 1); // case D
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_d = this->manhattanDistance(pos1, this->sherlock->getCurrentPosition()) + this->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_d = -1;
    }
    pos1 = this->pos;
    pos1.setCol(getCurrentPosition().getCol() + 1); // case R
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_r = this->manhattanDistance(pos1, this->sherlock->getCurrentPosition()) + this->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_r = -1;
    }
    maxx = (d_u > d_l) ? d_u : d_l;
    maxx = (maxx > d_d) ? maxx : d_d;
    maxx = (maxx > d_r) ? maxx : d_r;
    pos1 = this->pos;
    if (maxx != -1)
    {
        if (maxx == d_u)
        {
            pos1.setRow(this->getCurrentPosition().getRow() - 1);
            return pos1;
        }
        else if (maxx == d_l)
        {
            pos1.setCol(this->getCurrentPosition().getCol() - 1);
            return pos1;
        }
        else if (maxx == d_d)
        {
            pos1.setRow(this->getCurrentPosition().getRow() + 1);
            return pos1;
        }
        else if (maxx == d_r)
        {
            pos1.setCol(this->getCurrentPosition().getCol() + 1);
            return pos1;
        }
    }
    else
    {
        return Position::npos;
    }
    return Position::npos;
}

void Criminal::move()
{
    if (this->getNextPosition().getCol() != Position::npos.getCol() && this->getNextPosition().getRow() != Position::npos.getRow())
    {
        this->prev_pos = this->pos;
        this->pos = this->getNextPosition();
        this->count++;
    }
}

string Criminal::str() const
{
    return "Criminal[index=" + to_string(this->Criminal::index) + ";pos=" + this->Criminal::pos.str() + "]";
}

MovingCharacter Criminal::getCharacterType() const
{
    return MovingCharacter::CRIMINAL;
}

int Criminal::getCount() const
{
    return this->count;
}

bool Criminal::isCreatedRobotNext() const
{
    if (this->count > 0)
    {
        if (this->count % 3 == 0)
        {
            return true;
        }
    }
    return false;
}

Position Criminal::getPrevPosition() const
{
    return this->prev_pos;
}

/*
 * /---3.8---Class::ArrayMovingObject/
 */

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[this->capacity];
    for (int i = 0; i < this->capacity; ++i)
    {
        this->arr_mv_objs[i] = nullptr;
    }
}

ArrayMovingObject::~ArrayMovingObject()
{
    if (this->capacity == 0)
    {
        return;
    }

    for (int i = 3; i < this->capacity; ++i)
    {
        delete this->arr_mv_objs[i];
    }
    delete[] this->arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    return this->count == this->capacity;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    for (int i = 0; i < this->capacity; ++i)
    {
        if (this->arr_mv_objs[i] == nullptr)
        {
            this->arr_mv_objs[i] = mv_obj;
            this->count++;
            return true;
        }
    }
    return false;
}

string ArrayMovingObject::str() const
{
    string s = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity);
    for (int i = 0; i < this->capacity; ++i)
    {
        if (this->arr_mv_objs[i] != nullptr)
        {
            s = s + ";" + this->arr_mv_objs[i]->str();
        }
    }
    s = s + "]";
    return s;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    return this->arr_mv_objs[index];
}

int ArrayMovingObject::size() const
{
    /*int count1 = 0;
     for (int i = 0; i < this->capacity; ++i)
     {
         if (this->arr_mv_objs[i] != nullptr)
         {
             count1++;
         }
     }
     return count1;*/
    return this->count;
}

bool ArrayMovingObject::checkEvent(int index) const
{
    MovingObject *obj = this->get(index);
    MovingCharacter obj_type = obj->getCharacterType();
    bool boolcheck = false;
    for (int i = 0; i < this->size(); ++i)
    {
        if (i != index)
        {
            MovingObject *obj_event = this->get(i);
            MovingCharacter obj_event_type = obj_event->getCharacterType();
            if (obj->getCurrentPosition().getCol() == obj_event->getCurrentPosition().getCol() && obj->getCurrentPosition().getRow() == obj_event->getCurrentPosition().getRow())
            {
                if (obj_type == SHERLOCK)
                {
                    if (obj_event_type == ROBOT)
                    {
                        RobotType robot_type = dynamic_cast<Robot *>(obj_event)->getType();
                        switch (robot_type)
                        {
                        case S:
                            dynamic_cast<Sherlock *>(obj)->event(dynamic_cast<RobotS *>(obj_event));
                            break;
                        case W:
                            dynamic_cast<Sherlock *>(obj)->event(dynamic_cast<RobotW *>(obj_event));
                            break;
                        case SW:
                            dynamic_cast<Sherlock *>(obj)->event(dynamic_cast<RobotSW *>(obj_event));
                            break;
                        case C:
                            boolcheck = dynamic_cast<Sherlock *>(obj)->event(dynamic_cast<RobotC *>(obj_event));
                            if (boolcheck)
                            {
                                dynamic_cast<Sherlock *>(obj)->setPos(dynamic_cast<RobotC *>(obj_event)->getcreatepos());
                                return true;
                            }
                            break;
                        }
                    }
                    else if (obj_event_type == WATSON)
                    {
                        dynamic_cast<Sherlock *>(obj)->event(dynamic_cast<Watson *>(obj_event));
                        dynamic_cast<Watson *>(obj_event)->event(dynamic_cast<Sherlock *>(obj));
                    }
                    else if (obj_event_type == CRIMINAL)
                    {
                        return true;
                    }
                }
                else if (obj_type == WATSON)
                {
                    if (obj_event_type == ROBOT)
                    {
                        RobotType robot_type = dynamic_cast<Robot *>(obj_event)->getType();
                        switch (robot_type)
                        {
                        case S:
                            dynamic_cast<Watson *>(obj)->event(dynamic_cast<RobotS *>(obj_event));
                            break;
                        case W:
                            dynamic_cast<Watson *>(obj)->event(dynamic_cast<RobotW *>(obj_event));
                            break;
                        case SW:
                            dynamic_cast<Watson *>(obj)->event(dynamic_cast<RobotSW *>(obj_event));
                            break;
                        case C:
                            dynamic_cast<Watson *>(obj)->event(dynamic_cast<RobotC *>(obj_event));
                            break;
                        }
                    }
                    else if (obj_event_type == SHERLOCK)
                    {
                        dynamic_cast<Sherlock *>(obj_event)->event(dynamic_cast<Watson *>(obj));
                        dynamic_cast<Watson *>(obj)->event(dynamic_cast<Sherlock *>(obj_event));
                    }
                    else if (obj_event_type == CRIMINAL)
                    {
                        return true;
                    }
                }
                else if (obj_type == CRIMINAL)
                {
                    if (obj_event_type == ROBOT)
                    {
                        boolcheck = false;
                    }
                    else if (obj_event_type == SHERLOCK || obj_event_type == WATSON)
                    {
                        return true;
                    }
                }
                else if (obj_type == ROBOT)
                {
                    if (obj_event_type == ROBOT)
                    {
                        boolcheck = false;
                    }
                    else if (obj_event_type == SHERLOCK)
                    {
                        RobotType robot_type = dynamic_cast<Robot *>(obj)->getType();
                        switch (robot_type)
                        {
                        case S:
                            dynamic_cast<Sherlock *>(obj_event)->event(dynamic_cast<RobotS *>(obj));
                            break;
                        case W:
                            dynamic_cast<Sherlock *>(obj_event)->event(dynamic_cast<RobotW *>(obj));
                            break;
                        case SW:
                            dynamic_cast<Sherlock *>(obj_event)->event(dynamic_cast<RobotSW *>(obj));
                            break;
                        case C:
                            boolcheck = dynamic_cast<Sherlock *>(obj_event)->event(dynamic_cast<RobotC *>(obj));
                            if (boolcheck)
                            {
                                dynamic_cast<Sherlock *>(obj_event)->setPos(dynamic_cast<RobotC *>(obj)->getcreatepos());
                                return true;
                            }
                            break;
                        }
                    }
                    else if (obj_event_type == WATSON)
                    {
                        RobotType robot_type = dynamic_cast<Robot *>(obj)->getType();
                        switch (robot_type)
                        {
                        case S:
                            dynamic_cast<Watson *>(obj_event)->event(dynamic_cast<RobotS *>(obj));
                            break;
                        case W:
                            dynamic_cast<Watson *>(obj_event)->event(dynamic_cast<RobotW *>(obj));
                            break;
                        case SW:
                            dynamic_cast<Watson *>(obj_event)->event(dynamic_cast<RobotSW *>(obj));
                            break;
                        case C:
                            dynamic_cast<Watson *>(obj_event)->event(dynamic_cast<RobotC *>(obj));
                            break;
                        }
                    }
                    else if (obj_event_type == CRIMINAL)
                    {
                        boolcheck = false;
                    }
                }
            }
        }
    }
    return boolcheck;
}

/*
 * /---3.9---Class::Configuration/
 */

int Configuration::numvalue(string &s0, int &num)
{
    s0 = s0.substr(s0.find('=') + 1);
    if (!s0.empty())
    {
        stringstream ss(s0);
        ss >> num;
        s0.clear();
        ss.clear();
        return num;
    }
    return 0;
}

string Configuration::alphavalue(string &s0, string &s1)
{
    s1 = s0.substr(s0.find('=') + 1);
    s0.clear();
    return s1;
}

void Configuration::allocate2(int &a, Position *&arr)
{
    arr = new Position[a];
}

void Configuration::deallocate2(int &a, Position *arr)
{
    delete[] arr;
}

void Configuration::arr2(string &s0, int &a, Position *&arr)
{
    string s1;
    string result;
    a = 0;
    alphavalue(s0, s1);
    for (int i = 0; i < s1.length(); ++i)
    {
        if (s1[i] == '(')
        {
            a++;
        }
    }
    allocate2(a, arr);
    int ansx = 0, ansy = 0;
    int sub = 0;
    for (int i = 0; i < s1.length(); ++i)
    {
        if (isdigit(s1[i]))
        {
            result.push_back(s1[i]);
        }
        else if (!result.empty())
        {
            stringstream ss(result);
            ss >> sub;
            if (ansy == 0)
            {
                arr[ansx].setRow(sub);
                ansy++;
            }
            else if (ansy == 1)
            {
                arr[ansx].setCol(sub);
                ansy = 0;
                ansx++;
            }
            ss.clear();
            result.clear();
            sub = 0;
        }
    }
}

void Configuration::arr1(string &s0, Position &a)
{
    string s1, result;
    stringstream ss;
    int ans = 0;
    alphavalue(s0, s1);
    int ansy = 0, sub = 0;
    for (int i = 0; i < s1.length(); i++)
    {
        if (isdigit(s1[i]))
        {
            result.push_back(s1[i]);
        }
        else if (!result.empty())
        {
            stringstream ss(result);
            ss >> sub;
            if (ansy == 0)
            {
                a.setRow(sub);
                ansy++;
            }
            else if (ansy == 1)
            {
                a.setCol(sub);
                ansy = 0;
            }
            result.clear();
            ss.clear();
        }
    }
}

Configuration::Configuration(const string &filepath)
{
    this->map_num_rows = 0;
    this->map_num_cols = 0;
    this->max_num_moving_objects = 0;
    this->num_walls = 0;
    this->arr_walls = nullptr;
    this->num_fake_walls = 0;
    this->arr_fake_walls = nullptr;
    this->sherlock_moving_rule = "";
    this->sherlock_init_pos = Position::npos;
    this->sherlock_init_hp = 0;
    this->sherlock_init_exp = 0;
    this->watson_moving_rule = "";
    this->watson_init_pos = Position::npos;
    this->watson_init_hp = 0;
    this->watson_init_exp = 0;
    this->criminal_init_pos = Position::npos;
    this->num_steps = 0;
    ifstream ifs(filepath);
    if (ifs.is_open())
    {
        string s0;
        string s1;
        while (getline(ifs, s0))
        {
            s1 = s0.substr(0, s0.find('='));
            if (s1 == "MAP_NUM_ROWS")
            {
                this->numvalue(s0, this->map_num_rows);
            }
            else if (s1 == "MAP_NUM_COLS")
            {
                this->numvalue(s0, this->map_num_cols);
            }
            else if (s1 == "MAX_NUM_MOVING_OBJECTS")
            {
                this->numvalue(s0, this->max_num_moving_objects);
            }
            else if (s1 == "ARRAY_WALLS")
            {
                this->arr2(s0, this->num_walls, this->arr_walls);
            }
            else if (s1 == "ARRAY_FAKE_WALLS")
            {
                this->arr2(s0, this->num_fake_walls, this->arr_fake_walls);
            }
            else if (s1 == "SHERLOCK_MOVING_RULE")
            {
                this->alphavalue(s0, this->sherlock_moving_rule);
            }
            else if (s1 == "SHERLOCK_INIT_POS")
            {
                this->arr1(s0, this->sherlock_init_pos);
            }
            else if (s1 == "SHERLOCK_INIT_HP")
            {
                this->numvalue(s0, this->sherlock_init_hp);
            }
            else if (s1 == "SHERLOCK_INIT_EXP")
            {
                this->numvalue(s0, this->sherlock_init_exp);
            }
            else if (s1 == "WATSON_MOVING_RULE")
            {
                this->alphavalue(s0, this->watson_moving_rule);
            }
            else if (s1 == "WATSON_INIT_POS")
            {
                this->arr1(s0, this->watson_init_pos);
            }
            else if (s1 == "WATSON_INIT_HP")
            {
                this->numvalue(s0, this->watson_init_hp);
            }
            else if (s1 == "WATSON_INIT_EXP")
            {
                this->numvalue(s0, this->watson_init_exp);
            }
            else if (s1 == "CRIMINAL_INIT_POS")
            {
                this->arr1(s0, this->criminal_init_pos);
            }
            else if (s1 == "NUM_STEPS")
            {
                this->numvalue(s0, this->num_steps);
            }
        }
    }
}

Configuration::~Configuration()
{
    deallocate2(this->num_walls, this->arr_walls);
    deallocate2(this->num_fake_walls, this->arr_fake_walls);
}

string Configuration::str() const
{
    string wall = "[";
    for (int i = 0; i < num_walls; ++i)
    {
        wall = wall + arr_walls[i].str();
        if (i != num_walls - 1)
        {
            wall = wall + ";";
        }
    }
    wall = wall + "]";

    string fakewall = "[";
    for (int i = 0; i < num_fake_walls; ++i)
    {
        fakewall = fakewall + arr_fake_walls[i].str();
        if (i != num_fake_walls - 1)
        {
            fakewall = fakewall + ";";
        }
    }
    fakewall = fakewall + "]";

    string s;
    s = "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) + "\nMAP_NUM_COLS=" + to_string(map_num_cols) + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\nNUM_WALLS=" + to_string(num_walls) + "\nARRAY_WALLS=" + wall + "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS=" + fakewall + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\nSHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS=" + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP=" + to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(num_steps) + "\n]";
    return s;
}

/*
 * /---3.10---Class::Robot/
 */

Robot::Robot(int index, const Position &pos, Map *map, Criminal *criminal, const string &name)
    : MovingObject(index, pos, map, name)
{
    this->index = index;
    this->pos = pos;
    this->create_pos = pos;
    this->map = map;
    this->name = name;
    this->criminal = criminal;
    int i = this->pos.getRow();
    int j = this->pos.getCol();
    int p = i * j;
    int s = p;
    while (s > 9)
    {
        int sum = 0;
        while (s != 0)
        {
            sum += s % 10;
            s /= 10;
        }
        s = sum;
    }
    if (s == 0 || s == 1)
    {
        this->item = new MagicBook();
    }
    else if (s == 2 || s == 3)
    {
        this->item = new EnergyDrink();
    }
    else if (s == 4 || s == 5)
    {
        this->item = new FirstAid();
    }
    else if (s == 6 || s == 7)
    {
        this->item = new ExcemptionCard();
    }
    else if (s == 8 || s == 9)
    {
        this->item = new PassingCard(i, j);
    }
}

MovingCharacter Robot::getCharacterType() const
{
    return MovingCharacter::ROBOT;
}

Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    if (criminal->getCount() == 3)
    {
        return new RobotC(index, criminal->getPrevPosition(), map, criminal);
    }
    else if (criminal->isCreatedRobotNext())
    {
        int d_sher = criminal->manhattanDistance(criminal->getPrevPosition(), sherlock->getCurrentPosition());
        int d_wat = criminal->manhattanDistance(criminal->getPrevPosition(), watson->getCurrentPosition());
        if (d_sher > d_wat)
        {
            return new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
        }
        else if (d_sher < d_wat)
        {
            return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        }
        else if (d_sher == d_wat)
        {
            return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        }
    }
    else
    {
        return nullptr;
    }
    return nullptr;
}

BaseItem *Robot::NewItem()
{
    /*switch (this->item->getType())
      {
      case MAGIC_BOOK:
      {
          return new MagicBook();
          break;
      }
      case ENERGY_DRINK:
      {
          return new EnergyDrink();
          break;
      }
      case FIRST_AID:
      {
          return new FirstAid();
          break;
      }
      case EXCEMPTION_CARD:
      {
          return new ExcemptionCard();
          break;
      }
      case PASSING_CARD:
      {
          return new PassingCard(this->create_pos.getRow(), this->create_pos.getCol());
          break;
      }
      }
      return nullptr;*/
    return this->item;
}

/*-----Class::RobotC-----*/

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : Robot(index, init_pos, map, criminal, "RobotC")
{
    this->RobotC::index = index;
    this->RobotC::pos = init_pos;
    this->RobotC::map = map;
    this->RobotC::robot_type = RobotType::C;
    this->RobotC::criminal = criminal;
    this->RobotC::name = "RobotC";
}

int RobotC::getDistance(Sherlock *sherlock)
{
    return this->criminal->manhattanDistance(this->pos, sherlock->getCurrentPosition());
}

int RobotC::getDistance(Watson *watson)
{
    return this->criminal->manhattanDistance(this->pos, watson->getCurrentPosition());
}

Position RobotC::getCurrentPosition()
{
    return this->pos;
}

Position RobotC::getNextPosition()
{
    if (criminal->getPrevPosition().getCol() == criminal->getCurrentPosition().getCol() && criminal->getPrevPosition().getRow() == criminal->getCurrentPosition().getRow())
    {
        return Position::npos;
    }
    return this->criminal->getPrevPosition();
}

void RobotC::move()
{
    if (this->getNextPosition().getCol() != Position::npos.getCol() && this->getNextPosition().getRow() != Position::npos.getRow())
    {
        this->pos = this->getNextPosition();
    }
}

string RobotC::str() const
{
    return "Robot[pos=" + this->RobotC::pos.str() + ";type=C;dist=" + "]";
}

RobotType RobotC::getType() const
{
    return this->robot_type;
}

RobotC::~RobotC()
{
    delete this->item;
}

/*-----Class::RobotS-----*/

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(index, init_pos, map, criminal, "RobotS") // TODO: constructor class cha
{
    this->RobotS::index = index;
    this->RobotS::pos = init_pos;
    this->RobotS::map = map;
    this->RobotS::robot_type = RobotType::S;
    this->RobotS::criminal = criminal;
    this->RobotS::sherlock = sherlock;
    this->RobotS::name = "RobotS";
}

RobotS::~RobotS()
{
    delete this->item;
}

Position RobotS::getNextPosition()
{
    Position pos1 = this->getCurrentPosition();
    int d_u, d_r, d_d, d_l;
    pos1.setRow(this->getCurrentPosition().getRow() - 1); // case U
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_u = criminal->manhattanDistance(pos1, this->sherlock->getCurrentPosition());
    }
    else
    {
        d_u = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setCol(this->getCurrentPosition().getCol() - 1); // case L
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_l = criminal->manhattanDistance(pos1, this->sherlock->getCurrentPosition());
    }
    else
    {
        d_l = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setRow(this->getCurrentPosition().getRow() + 1); // case D
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_d = criminal->manhattanDistance(pos1, this->sherlock->getCurrentPosition());
    }
    else
    {
        d_d = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setCol(this->getCurrentPosition().getCol() + 1); // case R
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_r = criminal->manhattanDistance(pos1, this->sherlock->getCurrentPosition());
    }
    else
    {
        d_r = 1e7;
    }
    if (d_r == 1e7 && d_l == 1e7 && d_u == 1e7 && d_d == 1e7)
    {
        return Position::npos;
    }
    else
    {
        pos1 = this->pos;
        int minn = (d_u < d_l) ? d_u : d_l;
        minn = (minn < d_d) ? minn : d_d;
        minn = (minn < d_r) ? minn : d_r;
        if (minn == d_u)
        {
            pos1.setRow(this->getCurrentPosition().getRow() - 1);
            return pos1;
        }
        else if (minn == d_r)
        {
            pos1.setCol(this->getCurrentPosition().getCol() + 1);
            return pos1;
        }
        else if (minn == d_d)
        {
            pos1.setRow(this->getCurrentPosition().getRow() + 1);
            return pos1;
        }
        else if (minn == d_l)
        {
            pos1.setCol(this->getCurrentPosition().getCol() - 1);
            return pos1;
        }
    }
    return Position::npos;
}

void RobotS::move()
{
    if (this->getNextPosition().getCol() != Position::npos.getCol() && this->getNextPosition().getRow() != Position::npos.getRow())
    {
        this->pos = this->getNextPosition();
    }
}

int RobotS::getDistance() const
{
    return this->criminal->manhattanDistance(this->pos, this->sherlock->getCurrentPosition());
}

string RobotS::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=S;dist=" + to_string(this->getDistance()) + "]";
}

RobotType RobotS::getType() const
{
    return this->RobotS::robot_type;
}

/*-----Class::RobotW-----*/

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(index, init_pos, map, criminal, "RobotW") // TODO: constructor class cha
{
    this->RobotW::index = index;
    this->RobotW::pos = init_pos;
    this->RobotW::map = map;
    this->RobotW::watson = watson;
    this->RobotW::robot_type = RobotType::W;
    this->RobotW::criminal = criminal;
}

RobotW::~RobotW()
{
    delete this->item;
}

Position RobotW::getNextPosition()
{
    // TODO: tìm vị trí tiếp theo gần Watson nhất
    Position pos1 = this->getCurrentPosition();
    int d_u, d_r, d_d, d_l;
    pos1.setRow(this->getCurrentPosition().getRow() - 1); // case U
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_u = criminal->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_u = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setCol(this->getCurrentPosition().getCol() - 1); // case L
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_l = criminal->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_l = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setRow(this->getCurrentPosition().getRow() + 1); // case D
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_d = criminal->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_d = 1e7;
    }
    pos1 = this->getCurrentPosition();
    pos1.setCol(this->getCurrentPosition().getCol() + 1); // case R
    if (this->map->isValid(pos1, (MovingObject *)this))
    {
        d_r = criminal->manhattanDistance(pos1, this->watson->getCurrentPosition());
    }
    else
    {
        d_r = 1e7;
    }
    if (d_r == 1e7 && d_l == 1e7 && d_u == 1e7 && d_d == 1e7)
    {
        return Position::npos;
    }
    else
    {
        pos1 = this->pos;
        int minn = (d_u < d_l) ? d_u : d_l; // tìm số nhỏ nhất
        minn = (minn < d_d) ? minn : d_d;
        minn = (minn < d_r) ? minn : d_r;
        if (minn == d_u)
        {
            pos1.setRow(this->getCurrentPosition().getRow() - 1);
            return pos1;
        }
        else if (minn == d_r)
        {
            pos1.setCol(this->getCurrentPosition().getCol() + 1);
            return pos1;
        }
        else if (minn == d_d)
        {
            pos1.setRow(this->getCurrentPosition().getRow() + 1);
            return pos1;
        }
        else if (minn == d_l)
        {
            pos1.setCol(this->getCurrentPosition().getCol() - 1);
            return pos1;
        }
    }
    return Position::npos;
}

void RobotW::move()
{
    if (getNextPosition().getCol() != Position::npos.getCol() && getNextPosition().getRow() != Position::npos.getRow())
    {
        this->pos = this->getNextPosition();
    }
}

string RobotW::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=W;dist=" + to_string(this->getDistance()) + "]";
}

RobotType RobotW::getType() const
{
    return this->robot_type;
}

int RobotW::getDistance() const
{
    return criminal->manhattanDistance(this->pos, this->watson->getCurrentPosition());
}

/*-----Class::RobotSW-----*/

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(index, init_pos, map, criminal, "RobotSW") // TODO: constructor class cha
{
    this->RobotSW::index = index;
    this->RobotSW::pos = init_pos;
    this->RobotSW::map = map;
    this->RobotSW::robot_type = RobotType::SW;
    this->RobotSW::criminal = criminal;
    this->RobotSW::sherlock = sherlock;
    this->RobotSW::watson = watson;
    this->RobotSW::name = "RobotSW";
}

RobotSW::~RobotSW()
{
    delete this->item;
}

Position RobotSW::getNextPosition()
{
    Position pos1 = this->RobotSW::getCurrentPosition();
    int d_u, d_ur, d_r, d_dr, d_d, d_dl, d_l, d_ul;
    int min = 1e7;
    string c;
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 2); // case U
    if (this->RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_u = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        min = d_u;
        c = "u";
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 1); // case UR
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 1);
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_ur = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_ur < min)
        {
            min = d_ur;
            c = "ur";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 2); // case R
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_r = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_r < min)
        {
            min = d_r;
            c = "r";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 1); // case DR
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 1);
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_dr = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_dr < min)
        {
            min = d_dr;
            c = "dr";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 2); // case D
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_d = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());

        if (d_d < min)
        {
            min = d_d;
            c = "d";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 1); // case DL
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 1);
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_dl = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_dl < min)
        {
            min = d_dl;
            c = "dl";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 2); // case L
    if (this->RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_l = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_l < min)
        {
            min = d_l;
            c = "l";
        }
    }
    pos1 = this->RobotSW::getCurrentPosition();
    pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 1); // case UL
    pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 1);
    if (RobotSW::map->isValid(pos1, (MovingObject *)this))
    {
        d_ul = criminal->manhattanDistance(pos1, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(pos1, this->RobotSW::watson->getCurrentPosition());
        if (d_ul < min)
        {
            min = d_ul;
            c = "ul";
        }
    }
    if (min == 1e7)
    {
        return Position::npos;
    }
    else
    {
        pos1 = this->RobotSW::getCurrentPosition();
        if (c == "u")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 2);
            return pos1;
        }
        else if (c == "ur")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 1); // case UR
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 1);
            return pos1;
        }
        else if (c == "r")
        {
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 2);
            return pos1;
        }
        else if (c == "dr")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 1); // case DR
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() + 1);
            return pos1;
        }
        else if (c == "d")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 2);
            return pos1;
        }
        else if (c == "dl")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() + 1); // case DL
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 1);
            return pos1;
        }
        else if (c == "l")
        {
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 2);
            return pos1;
        }
        else if (c == "ul")
        {
            pos1.setRow(this->RobotSW::getCurrentPosition().getRow() - 1); // case UL
            pos1.setCol(this->RobotSW::getCurrentPosition().getCol() - 1);
            return pos1;
        }
    }
    return Position::npos;
}

string RobotSW::str() const
{
    return "Robot[pos=" + this->RobotSW::pos.str() + ";type=SW;dist=" + to_string(this->RobotSW::getDistance()) + "]";
}

RobotType RobotSW::getType() const
{

    return this->RobotSW::robot_type;
}

int RobotSW::getDistance() const
{
    return criminal->manhattanDistance(this->RobotSW::pos, this->RobotSW::sherlock->getCurrentPosition()) + criminal->manhattanDistance(this->RobotSW::pos, this->RobotSW::watson->getCurrentPosition());
}

void RobotSW::move()
{
    if (this->RobotSW::getNextPosition().getCol() != Position::npos.getCol() && this->RobotSW::getNextPosition().getRow() != Position::npos.getRow())
    {
        this->RobotSW::pos = this->RobotSW::getNextPosition();
    }
}

/*
 * /---3.11---Class::BaseItem/
 */

/*-----Class::MagicBook-----*/

ItemType MagicBook::getType() const
{
    return ItemType::MAGIC_BOOK;
}

MagicBook::~MagicBook()
{
    // delete this;
    // delete new MagicBook();
}

string MagicBook::str() const
{
    return "MagicBook";
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (obj->getCharacterType() == SHERLOCK)
    {
        if (dynamic_cast<Sherlock *>(obj)->getEXP() <= 350 && robot == nullptr)
        {
            return true;
        }
    }
    else if (obj->getCharacterType() == WATSON)
    {
        if (dynamic_cast<Watson *>(obj)->getEXP() <= 350 && robot == nullptr)
        {
            return true;
        }
    }
    return false;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (this->MagicBook::canUse(obj, robot))
    {
        if (obj->getCharacterType() == SHERLOCK)
        {
            dynamic_cast<Sherlock *>(obj)->setEXP(ceil(dynamic_cast<Sherlock *>(obj)->getEXP() * 1.25));
        }
        else if (obj->getCharacterType() == WATSON)
        {
            dynamic_cast<Watson *>(obj)->setEXP(ceil(dynamic_cast<Watson *>(obj)->getEXP() * 1.25));
        }
    }
}

/*-----Class::EnergyDrink-----*/

EnergyDrink::~EnergyDrink()
{
    //  delete this;
    //   delete new EnergyDrink();
}

ItemType EnergyDrink::getType() const
{
    return ItemType::ENERGY_DRINK;
}

string EnergyDrink::str() const
{
    return "EnergyDrink";
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if (obj->getCharacterType() == SHERLOCK)
    {
        if (dynamic_cast<Sherlock *>(obj)->getHP() <= 100 && robot == nullptr)
        {
            return true;
        }
    }
    else if (obj->getCharacterType() == WATSON)
    {
        if (dynamic_cast<Watson *>(obj)->getHP() <= 100 && robot == nullptr)
        {
            return true;
        }
    }
    return false;
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (this->EnergyDrink::canUse(obj, robot))
    {
        if (obj->getCharacterType() == SHERLOCK)
        {
            dynamic_cast<Sherlock *>(obj)->setHP(ceil(dynamic_cast<Sherlock *>(obj)->getHP() * 1.2));
        }
        else if (obj->getCharacterType() == WATSON)
        {
            dynamic_cast<Watson *>(obj)->setHP(ceil(dynamic_cast<Watson *>(obj)->getHP() * 1.2));
        }
    }
}

/*-----Class::FirstAid-----*/

FirstAid::~FirstAid()
{
    // delete new FirstAid();
}

ItemType FirstAid::getType() const
{
    return ItemType::FIRST_AID;
}

string FirstAid::str() const
{
    return "FirstAid";
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (obj->getCharacterType() == SHERLOCK)
    {
        if ((dynamic_cast<Sherlock *>(obj)->getHP() <= 100 || dynamic_cast<Sherlock *>(obj)->getEXP() <= 350) && robot == nullptr)
        {
            return true;
        }
    }
    else if (obj->getCharacterType() == WATSON)
    {
        if ((dynamic_cast<Watson *>(obj)->getHP() <= 100 || dynamic_cast<Watson *>(obj)->getEXP() <= 350) && robot == nullptr)
        {
            return true;
        }
    }
    return false;
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if (this->FirstAid::canUse(obj, robot))
    {
        if (obj->getCharacterType() == SHERLOCK)
        {
            dynamic_cast<Sherlock *>(obj)->setHP(ceil(dynamic_cast<Sherlock *>(obj)->getHP() * 1.5));
        }
        else if (obj->getCharacterType() == WATSON)
        {
            dynamic_cast<Watson *>(obj)->setHP(ceil(dynamic_cast<Watson *>(obj)->getHP() * 1.5));
        }
    }
}

/*-----Class::ExcemptionCard-----*/

ExcemptionCard::~ExcemptionCard()
{
    // delete this;
    //  delete new ExcemptionCard();
}

ItemType ExcemptionCard::getType() const
{
    return ItemType::EXCEMPTION_CARD;
}

string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getCharacterType() != SHERLOCK)
    {
        return false;
    }
    else if (dynamic_cast<Sherlock *>(obj)->getHP() % 2 != 0 && robot != nullptr)
    {
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character *obj, Robot *robot) {}

/*-----Class::PassingCard-----*/

PassingCard::~PassingCard()
{
    // delete this;
    //  delete new PassingCard(challenge);
    // delete new PassingCard();
}

PassingCard::PassingCard(int i, int j)
{
    int t = (i * 11 + j) % 4;
    switch (t)
    {
    case 0:
        this->challenge = "RobotS";
        break;
    case 1:
        this->challenge = "RobotC";
        break;
    case 2:
        this->challenge = "RobotSW";
        break;
    default:
        this->challenge = "all";
        break;
    }
}

PassingCard::PassingCard(string challenge)
{
    this->challenge = challenge;
}

ItemType PassingCard::getType() const
{
    return ItemType::PASSING_CARD;
}

string PassingCard::str() const
{
    return "PassingCard";
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getCharacterType() != WATSON)
    {
        return false;
    }
    else if (dynamic_cast<Watson *>(obj)->getHP() % 2 == 0 && robot != nullptr)
    {
        return true;
    }
    return false;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (this->PassingCard::canUse(obj, robot))
    {
        PassingCard(dynamic_cast<Watson *>(obj)->getCurrentPosition().getRow(), dynamic_cast<Watson *>(obj)->getCurrentPosition().getCol());
        if (this->challenge == "RobotC" && robot->getType() != RobotType::C)
        {
            dynamic_cast<Watson *>(obj)->setEXP(dynamic_cast<Watson *>(obj)->getEXP() - 50);
        }
        else if (this->challenge == "RobotS" && robot->getType() != RobotType::S)
        {
            dynamic_cast<Watson *>(obj)->setEXP(dynamic_cast<Watson *>(obj)->getEXP() - 50);
        }
        else if (this->challenge == "RobotW" && robot->getType() != RobotType::W)
        {
            dynamic_cast<Watson *>(obj)->setEXP(dynamic_cast<Watson *>(obj)->getEXP() - 50);
        }
        else if (this->challenge == "RobotSW" && robot->getType() != RobotType::SW)
        {
            dynamic_cast<Watson *>(obj)->setEXP(dynamic_cast<Watson *>(obj)->getEXP() - 50);
        }
    }
}

/*
 * /---3.12---Class::BaseBag/
 */

BaseBag::BaseBag(int capacity)
{
    this->capacity = capacity;
    this->size = 0;
    this->head = nullptr;
}

BaseBag::~BaseBag()
{
    Node *delNode = nullptr;
    while (this->size > 0)
    {
        delNode = this->head;
        head = this->head->next;
        // delete delNode->item;
        delete delNode;
        this->size--;
    }
    // delete obj;
}

bool BaseBag::insert(BaseItem *item)
{
    if (this->size == this->capacity)
    {
        return false;
    }
    else
    {
        Node *newNode = new Node(item, head);
        head = newNode;
        size++;
    }
    return true;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    Node *temp = head;
    BaseItem *save = nullptr;
    while (temp != nullptr)
    {
        if (temp->item->getType() == itemType)
        {
            save = temp->item;
            temp->item = head->item;
            size--;
            head = head->next;
            return save;
        }
        temp = temp->next;
    }
    return nullptr;
}

BaseItem *BaseBag::get()
{
    Node *temp = head;
    BaseItem *save = nullptr;
    while (temp != nullptr)
    {
        if (temp->item->canUse(obj, nullptr))
        {
            save = temp->item;
            temp->item = head->item;
            size--;
            head = head->next;
            return save;
        }
        temp = temp->next;
    }
    return nullptr;
}

string BaseBag::str() const
{
    string s;
    Node *temp = head;
    if (this->size == 0)
    {
        return "Bag[count=" + to_string(this->size) + ";]";
    }
    else
    {
        for (int i = 1; i <= size; ++i)
        {
            s += temp->item->str();
            if (i != size)
            {
                s += ",";
            }
            temp = temp->next;
        }
        return "Bag[count=" + to_string(size) + ";" + s + "]";
    }
}

bool BaseBag::checkItem(ItemType itemType)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->item->getType() == itemType)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

/*-----Class::SherlockBag-----*/

SherlockBag::SherlockBag(Sherlock *character)
    : BaseBag(13)
{
    this->sherlock = character;
}

SherlockBag::~SherlockBag()
{
    Node *delNode = nullptr;
    while (this->size > 0)
    {
        delNode = this->head;
        head = this->head->next;
        // delete delNode->item;
        delete delNode;
        this->size--;
    }
}

BaseItem *SherlockBag::get()
{
    Node *temp = head;
    BaseItem *save = nullptr;
    while (temp != nullptr)
    {
        if (temp->item->canUse(sherlock, nullptr))
        {
            save = temp->item;
            temp->item = head->item;
            size--;
            head = head->next;
            return save;
        }
        temp = temp->next;
    }
    return nullptr;
}

/*-----Class::WatsonBag-----*/

WatsonBag::WatsonBag(Watson *character)
    : BaseBag(15)
{
    this->watson = character;
}

WatsonBag::~WatsonBag()
{
    Node *delNode = nullptr;
    while (this->size > 0)
    {
        delNode = this->head;
        head = this->head->next;
        // delete delNode->item;
        delete delNode;
        this->size--;
    }
}

BaseItem *WatsonBag::get()
{
    Node *temp = head;
    BaseItem *save = nullptr;
    while (temp != nullptr)
    {
        if (temp->item->canUse(watson, nullptr))
        {
            save = temp->item;
            temp->item = head->item;
            size--;
            head = head->next;
            return save;
        }
        temp = temp->next;
    }
    return nullptr;
}

/*
 * /---3.13---Class::StudyInPinkProgram/
 */

StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete criminal;
    delete sherlock;
    delete watson;

    delete map;
    delete config;
    delete arr_mv_objs;
}

void StudyPinkProgram::run(bool verbose, ofstream &OUTPUT)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        if (istep == 0 && isStop())
        {
            break;
        }
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            MovingObject *robot = nullptr;
            arr_mv_objs->get(i)->move();
            boolcheck = arr_mv_objs->checkEvent(i);
            if (arr_mv_objs->get(i)->getCharacterType() == MovingCharacter::CRIMINAL)
            {
                robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            if (isStop())
            {
                printInfo(istep, i, OUTPUT);
                break;
            }
            if (robot != nullptr && criminal->getCount() % 3 == 0 && criminal->getCount() > 0)
            {
                arr_mv_objs->add(robot);
            }
            if (verbose)
            {
                printInfo(istep, i, OUTPUT);
            }
        }
        if (isStop())
        {
            break;
        }
    }
    printResult();
}

void StudyPinkProgram::run(bool verbose)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        if (istep == 0 && isStop())
        {
            break;
        }
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            MovingObject *robot = nullptr;
            arr_mv_objs->get(i)->move();
            boolcheck = arr_mv_objs->checkEvent(i);
            if (arr_mv_objs->get(i)->getCharacterType() == MovingCharacter::CRIMINAL)
            {
                robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            if (isStop())
            {
                printStep(istep);
                break;
            }
            if (robot != nullptr && criminal->getCount() % 3 == 0 && criminal->getCount() > 0)
            {
                arr_mv_objs->add(robot);
            }
            if (verbose)
            {
                printStep(istep);
            }
        }
        if (isStop())
        {
            break;
        }
    }
    printResult();
}

bool StudyPinkProgram::isStop() const
{
    if (boolcheck)
    {
        return true;
    }
    if (watson->getHP() <= 1 || sherlock->getHP() <= 1)
    {
        return true;
    }
    return false;
}