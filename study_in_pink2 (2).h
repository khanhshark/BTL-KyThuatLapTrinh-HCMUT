/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"
////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;
// addition
class Character;
class Watson;
class Sherlock;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

enum MovingCharacter
{
    SHERLOCK,
    WATSON,
    CRIMINAL,
    ROBOT
};

class MapElement
{
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
    friend class TestStudyInPink;

public:
    Path();
};

class Wall : public MapElement
{
    friend class TestStudyInPink;

public:
    Wall();
};

class FakeWall : public MapElement
{
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class Position
{
    friend class TestStudyInPink;

private:
    int r, c;

public:
    static const Position npos;
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingCharacter getCharacterType() const = 0;
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

public:
    Character(int index, const Position pos, Map *map, const string &name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingCharacter getCharacterType() const = 0;
};

class Sherlock : public Character
{
    friend class TestStudyInPink;

private:
    // TODO
    int hp, exp;
    // addition
    string moving_rule;
    string moving_rule_str;
    int index_moving_rule;
    BaseBag *bag;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    // addition
    MovingCharacter getCharacterType() const;
    BaseBag *getBag() const;
    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
    void setPos(Position pos);
    bool event(RobotS *robotS);
    bool event(RobotW *robotW);
    bool event(RobotSW *robotSW);
    bool event(RobotC *robotC);
    bool event(Watson *watson);
    ~Sherlock();
};

class Watson : public Character
{
    friend class TestStudyInPink;

private:
    // TODO
    int hp, exp;
    // addition
    string moving_rule;
    string moving_rule_str;
    int index_moving_rule;
    BaseBag *bag;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    // addition
    MovingCharacter getCharacterType() const;
    BaseBag *getBag() const;
    int getHP() const;
    int getEXP() const;
    void setHP(int hp);
    void setEXP(int exp);
    bool event(RobotS *robotS);
    bool event(RobotW *robotW);
    bool event(RobotSW *robotSW);
    bool event(RobotC *robotC);
    bool event(Sherlock *sherlock);
    ~Watson();
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    // TODO
    Sherlock *sherlock;
    Watson *watson;
    // addition
    int count;
    Position prev_pos;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    void move();
    string str() const;
    ~Criminal();
    // addition
    int manhattanDistance(const Position &pos1, const Position &pos2) const;
    MovingCharacter getCharacterType() const;
    int getCount() const;
    bool isCreatedRobotNext() const;
    Position getPrevPosition() const;
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    // TODO
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const;
    string str() const;
    bool checkEvent(int index) const;
};

class Configuration
{
    friend class TestStudyInPink;
    friend class StudyPinkProgram;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
    int numvalue(string &s0, int &num);
    string alphavalue(string &s0, string &s1);
    void allocate2(int &a, Position *&arr);
    void deallocate2(int &a, Position *arr);
    void arr2(string &s0, int &a, Position *&arr);
    void arr1(string &s0, Position &a);
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
    friend class TestStudyInPink;

protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;
    Position create_pos;

public:
    Robot(int index,
          const Position &pos,
          Map *map,
          Criminal *criminal,
          const string &name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType() { return this->robot_type; }
    // addition
    MovingCharacter getCharacterType() const;
    Position getcreatepos() { return criminal->getCurrentPosition(); }
    static Robot *create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    virtual int getDistance() const { return 0; }
    BaseItem *NewItem();
    virtual ~Robot() = default;
};

class RobotC : public Robot
{
    friend class TestStudyInPink;

private:
public:
    RobotC(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal);
    Position getNextPosition();
    Position getCurrentPosition();
    void move();
    int getDistance(Sherlock *sherlock);
    int getDistance(Watson *watson);
    string str() const;
    // addition
    RobotType getType() const;
    ~RobotC();
};

class RobotS : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;

public:
    RobotS(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal,
           Sherlock *sherlock);

    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
    // addition
    RobotType getType() const;
    ~RobotS();
};

class RobotW : public Robot
{
    friend class TestStudyInPink;

private:
    Watson *watson;

public:
    RobotW(int index,
           const Position &init_pos,
           Map *map,
           Criminal *criminal,
           Watson *watson);
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
    // addition
    RobotType getType() const;
    ~RobotW();
};

class RobotSW : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index,
            const Position &init_pos,
            Map *map,
            Criminal *criminal,
            Sherlock *sherlock,
            Watson *watson);

    Position getNextPosition();
    string str() const;
    int getDistance() const;
    void move();
    // addition
    RobotType getType() const;
    ~RobotSW();
};

class BaseItem
{
    friend class TestStudyInPink;

public:
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    // addition
    BaseItem() {}
    virtual ItemType getType() const = 0;
    virtual string str() const = 0;
    virtual ~BaseItem() = default;
};

class MagicBook : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    // addition
    ItemType getType() const;
    string str() const;
    ~MagicBook();
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    // addition
    ItemType getType() const;
    string str() const;
    ~EnergyDrink();
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    // addition
    ItemType getType() const;
    string str() const;
    ~FirstAid();
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    // addition
    ItemType getType() const;
    string str() const;
    ~ExcemptionCard();
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;

private:
    string challenge;

public:
    PassingCard(int i, int j);
    PassingCard(string challenge);
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    // addition
    ItemType getType() const;
    string str() const;
    ~PassingCard();
};

class BaseBag
{
    friend class TestStudyInPink;

protected:
    // addition
    class Node
    {
    public:
        BaseItem *item;
        Node *next;

    public:
        Node(BaseItem *item, Node *next = nullptr) : item(item), next(next) {}
    };

protected:
    Character *obj;
    // addition
    int size;
    int capacity;
    Node *head;

public:
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;

    // addition
    BaseBag(int capacity);
    ~BaseBag();
    bool checkItem(ItemType itemType);
    bool isFull() const;
};
// addition
class SherlockBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;

public:
    SherlockBag(Sherlock *character);
    BaseItem *get();
    ~SherlockBag();
};
// addition
class WatsonBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Watson *watson;

public:
    WatsonBag(Watson *character);
    BaseItem *get();
    ~WatsonBag();
};

class StudyPinkProgram
{
    friend class TestStudyInPink;

private:
    // Sample attributes
    Configuration *config;
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;
    Map *map;
    ArrayMovingObject *arr_mv_objs;
    // addition
    string outputFile;
    bool boolcheck = false;

public:
    StudyPinkProgram(const string &config_file_path);
    bool isStop() const;
    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
    ~StudyPinkProgram();
    // addition
    void run(bool verbose, ofstream &OUTPUT);
    void printInfo(int si, int i, ofstream &OUTPUT)
    {
        OUTPUT << endl
               << "*************AFTER MOVE*************" << endl;
        OUTPUT
            << "ROUND : " << si << " - TURN : " << i << endl;
        stringstream ss(arr_mv_objs->str());
        string lineArr = "";
        getline(ss, lineArr, 'C');
        OUTPUT << lineArr << "]" << endl;
        getline(ss, lineArr, ']');
        OUTPUT << "\tC" << lineArr << "]" << endl;
        while (getline(ss, lineArr, ']'))
        {
            if (lineArr.length() > 0)
                OUTPUT << "\t" << lineArr.substr(1) << "]" << endl;
        }
        OUTPUT << "Sherlock HP_" << sherlock->getHP() << " EXP_" << sherlock->getEXP() << endl
               << "Watson HP_" << watson->getHP() << " EXP_" << watson->getEXP() << endl
               << "SherlockBag : " << sherlock->getBag()->str() << endl
               << "WatsonBag : " << watson->getBag()->str() << endl;
    }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
