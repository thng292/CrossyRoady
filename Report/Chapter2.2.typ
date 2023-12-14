== GameMap
Trò chơi được thiết lập và thực thi tại màn hình GameMap. Cũng như các màn hình khác, class GameMap là một lớp con được kế thừa từ lớp cha AbstractScreen. Class GameMap sẽ là nơi lưu trữ những thứ liên quan đến trò chơi như trạng thái nhân vật, trạng thái các đường đi, trạng thái màn chơi,… Khi được khởi tạo (Init), điều đầu tiên được thực hiện là gán các giá trị được truyền từ các màn hình trước (màn hình chọn map màn hình chọn nhân vật), vào biến gameData, và dựa vào đây ta sẽ load những tài nguyên tương ứng (cụ thể là các sprite và anisprite của màn đấu và nhân vật được chọn). Cách thức load tài nguyên dựa vào các giá trị tương ứng sẽ giúp ta tạo được các màn chơi khác nhau ngay trong class GameMap mà không cần phải viết thêm class mới.
```Cpp
class GameMap : public ConsoleGame::AbstractScreen {
    std::vector<std::unique_ptr<Lane>> laneList;  
    Character character;                          
    Item mapItem;                                 
    Lane* laneWithItem;                           

    GameType::GameMapData gameData;            
    GameMaster::GameEventsArgs gameEventArgs;  
    GameMaster::GameFlags gameFlags;           
    //...
    
    void CheckCollision(float deltaTime);
    void UpdateLanes(float deltaTime);
    //...
    
    void DragMapDown(float deltatime);
    void DrawFlat(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawEntity(ConsoleGame::AbstractCanvas* canvas) const;
    void DrawHealth(ConsoleGame::AbstractCanvas* canvas) const;
    //...
```
Về các phương thức, GameMap gồm hai thành phần: vẽ và cập nhật. Các hàm vẽ (DrawFlat, DrawEntity,...) được gọi trong hàm Draw, và các hàm cập nhật (DragMapDown, UpdateLane, ...) được gọi trong hàm Update. Hai thành phần này có sự liên kết chặt chẽ với nhau, hàm Update sẽ cập nhật trạng thái của màn chơi, từ đó hàm Draw sẽ hiển thị trạng thái tương ứng cho người chơi qua giao diện trò chơi.
=== Class Character
Class Character đóng vai trò lưu trữ những thuộc tính cơ bản của nhân vật như vị trí, sprite, máu, tốc độ nhân vật,... Và các thuộc tính đó sẽ được thay đổi, truy cập qua các phương thức Set (hay Move đối với việc di chuyển), Get tương ứng. Chú ý rằng ta không tiếp nhận input của người chơi trong class này, mà việc ấy sẽ được xử lý trong class GameMap. Hay nói cách khác, đối tượng Character không tự hoạt động, mà sẽ được điều khiển qua GameMap. 

Tuy trò chơi có tổng cộng 6 nhân vật người chơi có thể lựa chọn, song chỉ có một class (class Character) để quản lý tất cả các nhân vật ấy. Lí do ta không tạo 6 class con riêng kế thừa từ class Character là vì tất cả các nhân vật đều có thuộc tính và phương thức giống nhau. Ngoài ra, kĩ năng của từng nhân vật sẽ được xử lý trong class GameMap thông qua CharaType của nhân vật.
```Cpp
class Character {
   private:
    GameType::CharaType _type;
    int maxHealth;
    int curHealth;
    float _speed;
    //...
    public: 
    void MoveLeft(float dist);
    void MoveRight(float dist);
    void MoveUp(float dist);
    void MoveDown(float dist);
    //...
```
=== Class Lane
Một trong các yếu tố chính của các trò chơi băng qua đường là các làn đường. Vì việc xử lý của làn đường là một trong những yếu tố quan trọng và phức tạp hơn của trò chơi, nên ta sẽ tách chúng riêng sang một class khác, cụ thể là class Lane.  
// Code class lane
```Cpp
class Lane {
   protected:
    GameType::LaneType _type;
    std::vector<float> entityList;
    bool IsLeftToRight;
    bool _hasItem = false;
    float speed;
    float laneY;
    ConsoleGame::Sprite* _laneSprite;
    //...

   public:
   //...
    virtual void Init();
    virtual void DeleteEntity();
    virtual void CreateEntity();
    virtual void UpdatePos(float deltaTime);
    //...
```
Class Lane là một virtual class, chứa nhiều thuộc tính phục vụ cho việc xử lý làn đường. Trong đó, thuộc tính entityList và type là hai thuộc tính quan trọng nhất. Thuộc tính entityList sẽ lưu trữ vị trí của các entity theo trục x, và type sẽ cho biết làn đường đó là loại đường nào. Từ đây, chúng ta sẽ tạo các lớp con Road, Rail, Water và SafeZone, tương ứng cho bốn loại làn đường khác nhau trong trò chơi. Một điều đáng chú ý về thuộc tính entityList là thuộc tính này có dạng vector<float>, không phải vector<Mob> hay vector<Entity>,… Đó là vì một làn đường chỉ sẽ chứa một loại quái/vật cản (gọi chung là entity), nên việc tạo ra một class riêng cho các entity để chứa vị trí và sprite của chúng sẽ khá tốn kém (data của sprite khá nặng). Chính vì vậy, chúng ta sẽ cho đối tượng làn đường quản lý sprite của các entity, từ đó trạng thái của các entity sẽ chỉ cần được lưu trữ qua một danh sách các vị trí của chúng.   
```Cpp
class Rail : public Lane {
   private:
    GameType::MobType _type;
    ConsoleGame::AniSprite* _mobSprite;
  //...
};
```
Các làn đường sẽ được đẩy và lưu vào thuộc tính laneList của GameMap. Vì laneList có dạng ```Cpp vector<unique_ptr<Lane>>laneList```, và các class Road, Rail, Water, SafeZone đều là lớp con của class Lane, nên ta có thể đẩy trực tiếp các lớp đó vào laneList, từ đó giúp việc cập nhật và xử lý danh sách các làn đường một cách dễ dàng hơn. 
==== Cập nhật entityList
Khi được khởi tạo, vị trí của các entity sẽ được random (dựa vào phần tử cuối cùng trong entityList) và đẩy vào entitylist. Cứ mỗi frame, hàm UpdatePos sẽ được gọi và cập nhật các giá trị của entityList theo hướng đã được định trước. Khi một phần tử của entityList vượt khỏi biên màn hình, ta se xóa phần tử đó đi (tương ứng việc xóa entity đó khỏi làn đường), và đẩy một phần tử mới vào cuối entityList (tương ứng việc thêm entity vào làn đường). Từ đó, tạo nên hiệu ứng làn đường luôn có quái chạy sang.
#figure(image("asset/entityList.png", width: 80%), caption: "Câp nhật làn đường (xóa quái bên phải, thêm quái bên trái)")
Khi dịch chuyển các entity, chú ý ta không dịch chuyển chúng theo khoảng cách cố định, mà quảng đường dịch chuyển sẽ được tính theo công thức `vận tốc làn * thời gian giữa frame` (độ dịch chuyển của nhân vật cũng được tính như vậy). Điều này sẽ đảm bảo tính ổn định của màn chơi qua các máy tính khác nhau. Chẳng hạn giữa hai máy có 110fps và 60fps, sau một khoảng thời gian t nào đó, thì độ dịch chuyển của quái của hai máy sẽ như nhau, không có sự chênh lệch.
==== Kiểm tra va chạm
Mỗi entity trong trò chơi sẽ có một hitbox riêng, nên để kiểm tra hai entity có va chạm hay không, ta chỉ cần kiểm tra hitbox của chúng có giao nhau hay không. Một hitbox đơn thuần là một hình chữ nhật bao quanh một entity, hình chữ nhật sẽ khác nhau tùy thuộc vào kích thước của sprite. Sau khi có được va chạm, ta sẽ kiểm tra loại va chạm đó thuộc loại đường nào. Cụ thể, nếu va chạm xuất hiện trên đường Road hoặc Rail, đó là va chạm giữa nhân vật và quái, nếu va chạm xuất hiện trên đường nước, đó là va chạm giữa nhân vật và nước, nếu va chạm xuất hiện trên SafeZone, đó là va chạm giữa nhân vật và vật cản. Dựa vào dạng va chạm mà ta sẽ có các cách xử lý khác nhau.
#figure(image("asset/hitbox.png", width:50%), caption: "Hitbox giữa nhân vật và quái")
// code
```Cpp
CollisionType Lane::GetCollision(const Character& character) const
{
    size_t listSize = entityList.size();
    Box charaBox = character.GetHitBox();
    for (size_t i = 0; i < listSize; ++i) {
        Box entityBox = GetHitBox(i);
        CollisionType colType = GetCollisionType(charaBox, entityBox);
        if (colType != CollisionType::None) {
            return colType;
        }
    }
    return CollisionType::None;
}
```
Lưu ý trong đoạn code trên, CollisionType không biểu thị đó là loại va chạm giữa nhân vật với quái, nhân vật với vật cản,... Mà nó biểu thị sự va chạm ở mặt nào của hitbox. Lí do vì sao ta cần biết thông tin này sẽ được giải thích ở phần sau.
```Cpp
enum CollisionType { None, Left, Right, Top, Bottom,... };
```
==== Làn Water
Làn Water là một loại làn đặc biệt hơn so với các làn còn lại. Khi kiểm tra va chạm, đối với làn Road, Rail và SafeZone, ta sẽ kiểm tra sự giao nhau giữa các hitbox của các entity và nhân vật. Nhưng đối với làn Water, ta sẽ kiểm tra sự giao nhau giữa hitbox của chính làn đó với nhân vật, chứ không phải giữa entity và nhân vật. Khi quan sát hình sau, ta sẽ dễ hình dung hơn lí do cho cách xử lý này.
#figure(image("asset/water.png"), caption: "Hitbox của làn Water và nhân vật")
Như hình trên, hitbox của làn Water sẽ là các khoảng trống giữa những miếng gỗ. Các "khoảng trống" ấy lúc này sẽ đóng vai trò nhưng những vật cản, không cho phép người chơi di chuyển qua khu vực ấy. Ngược lại, những miếng gỗ lúc này sẽ đóng vai trò như những đường đi, cho phép người chơi băng qua bình thường. Khi người chơi đúng trên miếng gỗ (nhân vật đang thuộc làn nước), nhân vật sẽ trôi theo chiều của miếng gỗ của làn nước ấy.   
== Thứ tự vẽ 
Thứ tự vẽ của các đối tượng sẽ như sau: Mặt phẳng -> Entity, vẽ từ cuối đến đầu của laneList. Điều này sẽ đảm bảo các đối tượng được hiển thị theo đúng thứ tự, vật đứng trước sẽ che vật đứng sau (vật có tọa độ y cao hơn sẽ được vẽ trước). Lưu ý, vì đối tượng Character không thuộc trong laneList, nên khi vẽ ra đường làn đường, ta cần so sánh tọa độ y giữa nhân vật và làn đường đang vẽ để xét thứ tự vẽ đúng đắn.
#figure(image("asset/draworder.png", width:50%), caption:"Thứ tự từ trên xuống (nhân vật -> vật cản -> quái)")
=== Kéo màn chơi xuống
Cách thức kéo màn chơi xuống khi người chơi tiến qua các làn đường rất đơn giản: khi người chơi vượt qua giữa màn hình (theo chiều y), ta sẽ chọn màn chơi một vật tốc bằng với vận tốc đi lên của người chơi, sau đó, ta cho từng làn đường dịch chuyển xuống với vận tốc ấy, từ đó tạo cảm giác màn chơi đang được kéo xuống. Ngoài ra, ta cũng cần phải cập nhật danh sách làn đường. Khi làn đường ở đầu laneList (làn đường dưới cùng màn hình), vượt khỏi biên màn hình, ta sẽ xóa làn đường đó đi mà thêm làn đường mới vào laneList. Làn đường mới sẽ được random một trong bốn làn đường đã nêu trên. Có thể thấy cách xử lý này rất giống với cách xử lý entityLít của làn đường.
#figure(image("asset/mapdrag.png", width:80%), caption:"Quá trình kéo màn chơi xuống")
=== Vật phẩm
Vật phẩm được biểu thị quá đối tượng Item. Class Item sẽ sơ hữu các thuộc tính về tọa độ, sprite và loại vật phẩm. Giống với class Character, ta không tạo class con riêng kế thừa từ class Item cho từng vật phẩm, vì các vật phẩm đều có phương thức giống nhau. Vật phẩm được khởi tạo cùng với một làn đường hiện có (điều này giải thích cho thuộc tính hasItem của class Lane), hay nói cách khác, vật phẩm sẽ nằm trên một làn đường nhất định. Lí do ta không cho vị trí vật phẩm là một tọa độ bất kì trên màn chơi là vì việc xử lý thứ tự vẽ khi ấy sẽ khó khăn hơn. Nhớ rằng Character, Item, và Lane đều là các class riêng biệt, không có mối liên hệ với nhau nên ta không thể cho chúng vào cùng một danh sách. Do đó, nếu ta coi vật phẩm như một entity của một làn đường, thì ta sẽ có thể xét thứ tự vẽ theo cách đã nêu trên.
#figure(image("asset/item.png", width: 50%), caption: "Vật phẩm máu trên làn Rail")
Xử lý va chạm vật phẩm và nhân vật cũng giống như cách xử lý va chạm đã nêu, khi người chơi va chạm vật phẩm, ta sẽ dựa vào loại vật phẩm mà có cách xử lý tương ứng. 
=== Kiểm soát trạng thái trò chơi
==== GameMaster
Để kiểm soát trạng thái hiện tại của trò chơi, ta sẽ dùng “cờ” và các “biến trò chơi”, cụ thể là struct GameFlags và GameEventArgs.
===== GameFlags
Mỗi hành động trong trò chơi sẽ được cụ thể hóa bằng một cờ tương ứng. Ta có hai loại cờ khác nhau, cờ biểu thị và cờ cho phép. Việc bật và tắt một cờ biểu thị sẽ tương ứng cho hành động đó có đang được thực hiện hay không, và việc bật và tắt một cờ cho phép sẽ tương ứng cho việc cho phép hoặc không cho phép một hành động trong trò chơi. Ví dụ, nếu người chơi đang di chuyển sang phải, cờ movingRight sẽ có giá trị true, nhưng cờ movingLeft, movingDown, movingUp sẽ có giá trị false. Nếu người chơi không được phép sử dụng kĩ năng, cờ allowSkill sẽ mang giá trị false, vv. Việc sử dụng cờ sẽ giúp ta xử lý các hành động, sự kiện trong trò chơi một cách dễ dàng hơn, đặc biệt là đối với skill và debuff. Ví dụ, khi debuff “ngưng chuyển động” được áp dụng, ta chỉ cần tắt cờ allowPlayerMovement, vv.
```Cpp
struct GameFlags {
        // Collision
        bool damageCollision : 1 = false;
        bool logCollision : 1 = false;
        bool itemCollision : 1 = false;
        bool blockCollision : 1 = false;

        // Keys
        bool allowMovementKeys : 1 = true;
        bool allowSkillKey : 1 = true;
        bool allowKeyLeft : 1 = true;
        bool allowKeyRight : 1 = true;
        bool allowKeyUp : 1 = true;
        bool allowKeyDown : 1 = true;
        //...
}
```
Như đã nói ở phần trước, khi xảy ra va chạm giữa nhân vật và entity, ta cần biết CollisionType của va chạm đó là gì. Thông tin này sẽ phục vụ cho việc tắt các cờ tương ứng khi người chơi gặp phải vật cản, hay gặp phải vùng nước.
```Cpp
// Xác định trạng thái cờ
void GameMap::HandleCollision(Lane* lane, CollisionType colType)
{
    //...
    switch (lane->GetType()) {
        //...
        case SAFE:
            gameFlags.blockCollision = true;
            if (colType == CollisionType::Left) {
                gameFlags.allowMoveRight = false;
            } else if (colType == CollisionType::Right) {
                gameFlags.allowMoveLeft = false;
            } else if (colType == CollisionType::Top) {
                gameFlags.allowMoveDown = false;
            } else if (colType == CollisionType::Bottom) {
                gameFlags.allowMoveUp = false;
            }
            break;
    }
}

// Dựa vào cờ để xử lý
void GameMap::HandlePlayerMovement(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    float distanceX = character.getSpeed() * deltaTime;
    if (gameFlags.movingLeft && gameFlags.allowMoveLeft) {
        character.MoveLeft(distanceX);
      }
    //...
}
```
===== GameEventArgs
Ngoài ra, ta còn có một biến để lưu những giá trị của trạng thái trò chơi. Nơi đây sẽ lưu các giá trị như điểm số, thời gian cooldown hiện tại của skill,… Các giá trị này, kết hợp với gameFlags, đóng vai trò như những biến toàn cục kiểm soát toàn bộ trạng thái của màn chơi. 
```Cpp
struct GameEventsArgs {
      // Map arguments
      float mapSpeedY;
      float mapSpeedX;
      float timeLeft;
      
      // Results
      size_t currentScore;
      size_t damageTaken;
      //...
}
```
==== Kĩ năng và hiệu ứng môi trường
Các kĩ năng của từng nhân vật và các hiệu ứng môi trường của mỗi màn chơi đều được thực hiện qua việc tác động và chỉnh sửa GameFlags và GameEventArgs. Thường quá trình xử lý một kĩ năng/hiệu ứng môi trường sẽ diễn ra như sau: trước hết, ta xác định loại kĩ năng/hiệu ứng ấy là loại nào (sử dụng switch case), từ đó ta sẽ bật tắt các cờ tương ứng. Đồng thời, lưu trữ, chỉnh sửa những biến cần thiết trong GameEventArgs. 
==== Xử lý tạm ngưng màn chơi
Để cài đặt chức năng ngưng màn chơi, ta chỉ cờ thêm một loại cờ isGamePaused, và chỉ cho phép các hàm cập nhật của GameMap chạy khi cờ này ở trạng thái tắt. Để tạo được hiệu ứng các menu nằm chồng lên màn chơi hiện tại, ta sẽ xử lý giống với màn hình HowToPlay. Cụ thể hơn, trong GameMap sẽ có một menu tạm ngưng, và menu chỉ được hiển thị khi cờ isGamePaused có giá trị true. Trong menu sẽ có các lựa chọn khác nhau, trong đó ta sẽ chú ý hơn đối với các lựa chọn sử dụng các màn hình khác (ví dụ như Setting). Vì để giữ được hình ảnh của màn chơi, ta không thể navigate sang một màn hình khác, chính vì vậy, ta sẽ gọi trực tiếp hàm Update và Draw của các màn hình ấy khi màn hình ấy được chọn. 

Vì xuyên suốt quá trình xử lý tạm ngưng, mọi hoạt động vẫn xảy ra trong màn hình GameMap, nên ta sẽ bảo lưu được hình ảnh của màn chơi.
#figure(image("asset/pause.png", width: 80%), caption: "Màn hình setting khi tạm ngưng")
==== Màn hình kết quả
Trò chơi sẽ kết thúc khi: nhân vật hết máu, nhân vật vượt khỏi biên, nhân vật hết thời gian (đối với chế độ thời gian). Khi đó, người chơi sẽ được dẫn đến màn hình kết quả, nơi hiện các thông số liên quan đến màn chơi vừa kết thúc. Như đã nói trên, GameEventArgs sẽ lưu trữ dữ liệu liên quan đến sự kiện của màn chơi, chính vì vậy, để hiển thị thông số kết quả, ta chỉ cần trích những số liệu cần thiết và truyền đến màn hình kết quả.
#figure(image("asset/result.png", width: 80%), caption: "Màn hình kết quả")
Sau khi màn chơi kết thúc, các dữ liệu như thời gian chơi, điểm EXP của màn, quãng đường di chuyển,... sẽ được cập nhật vào thông số tổng quát của trò chơi (ở màn hình Statistics). Màn hình kết quả cũng có vai trò kiểm tra và cập nhật tiến độ của người chơi, cụ thể là level và những nhân vật đã được mở khóa. 

=== Lưu và load trạng thái trò chơi
Sau khi xác nhận lưu màn chơi (khi tạm ngưng), người chơi sẽ được đưa về màn hình chọn map, hay nói cách khác, người chơi sẽ không được tiếp tục chơi màn chơi hiện tại sau khi đã xác nhận lưu. Đây là một cách thức lưu phổ biến của nhiều game như Minecraft, Dark Souls,... Ngoài ra, người chơi chỉ được cho phép lưu một màn chơi duy nhất. Lí do ta lựa chọn cách thức lưu này là để tránh việc người chơi lạm dụng màn chơi đã lưu để tích lũy điểm kinh nghiệm. Sau khi màn chơi đã được load, file save đã sử dụng sẽ được xóa đi.

#figure(image("asset/asksave.png", width: 80%), caption: "Màn hình xác nhận việc lưu và thoát màn chơi")
Vì trạng thái của trò chơi đã được lưu lại một cách cụ thể qua GameFlags, GameEventArgs,… ta chỉ cần lưu giá trị của các biến này. Một điều đang chú ý là ta sẽ không lưu giá trị của các sprite, vì chúng sẽ được xác định và load dựa trên các giá trị nhưng laneType, mobType,… Tóm lại, file lưu sẽ có cấu trúc như sau: thời gian được lưu, dữ liệu màn chơi (map nào, chế độ nào,...), thông tin nhân vật, thông tin vật phẩm hiện có, GameEventArgs, GameFlags, số làn hiện có, thông tin của các làn ấy.
#figure(image("asset/savefile.png", width: 80%), caption: "Cấu trúc file lưu màn chơi")
Khi load lại màn chơi, ngoài việc đọc và ghi lại các dữ liệu tương ứng cho từng biến của GameMap, ta phải lưu ý hơn trong việc tạo lại các làn đường đã lưu. Như đã nói trên, ta sẽ không lưu thông tin sprite/anisprite của làn đường, chính vì vậy, khi màn chơi được load, ta cần gán các sprite/anisprite tương ứng cho từng làn đường dựa trên thông tin được lưu.