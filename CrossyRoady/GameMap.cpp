#include "GameMap.h"

#include "SaveLoad.cpp"
using namespace ConsoleGame;
using namespace GameUtils;
using namespace GameType;
using namespace GameMaster;

void GameMap::InitLaneList()
{
    int initSafeCount = 3;
    for (size_t i = 0; i < initSafeCount; ++i) {
        laneList.push_back(std::make_unique<SafeZone>(
            32 * (i + 1),
            &gameSprites.safeSprite,
            &gameSprites.blockSprite,
            true,
            true
        ));
    }

    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    int maxLane = screenHeight / 32 + 1;
    for (size_t i = 0; i < maxLane; ++i) {
        laneList.push_back(GetRandomLane());
    }
}

const std::wstring_view GameMap::ScreenName() { return L"GameMap"; }

std::wstring_view GameMap::getName() { return ScreenName(); }

void GameMap::Init(const std::any& args)
{
    if (args.has_value()) {
        try {
            const GameType::GameMapData& gameDataArg =
                std::any_cast<const GameMapData&>(args);
            SetGameMapData(gameDataArg);
        } catch (const std::exception& e) {
            loadSave = std::any_cast<bool>(args);
        }
    }

    GameMapData gm;
    gm.charaType = KRONII;
    gm.mapMode = INF;
    gm.mapType = DESERT;
    gm.mapDifficulty = MNORMAL;

    SetGameMapData(gm);
    if (gameData.mapDifficulty != MPROG) {
        gameEventArgs.mobRange = gameData.mapDifficulty;
        gameEventArgs.mapDragSpeed =
            gameData.mapDifficulty == MHARD ? MAP_DRAG_SPEED : 0.0f;
        gameEventArgs.difficultyReached =
            static_cast<MobType>(gameData.mapDifficulty - 1);
    } else {
        gameEventArgs.mobRange = 1;
    }

    gameEventArgs.timeLeft = 10;
    gameEventArgs.skillCharge = MAX_SKILL_CHARGE;
    gameFlags.allowDebuff = false;
    // gameEventArgs.currentScore = 300;

    menu.Init(
        {20, 80},
        {130, 18},
        {
            R.String.Back,
            R.String.Setting.Title,
            R.String.Pause.ReturnMenu,
        }
    );
    subScreen[0] = std::make_unique<Setting>();
    subScreen[1] = std::make_unique<ReturnHome>();
    subScreen[2] = std::make_unique<HowToPlay>();
    subScreen[3] = std::make_unique<Credit>();
    for (auto& tmp : subScreen) {
        tmp->Init(std::any());
    }
}

AbstractScreen* GameMap::Clone() const { return new GameMap; }

AbstractNavigation::NavigationRes GameMap::Update(
    float deltaTime, const AbstractNavigation* navigation
)
{
    auto res = navigation->NoChange();

    ResetFlags();

    UpdateMapSpeed();
    DragMapDown(deltaTime);
    UpdateSprites(deltaTime);
    UpdateDifficulty();
    UpdateLanes(deltaTime);
    UpdateCooldowns(deltaTime);
    UpdateTime(deltaTime);

    CheckGameOver();
    CheckCollision(deltaTime);
    CheckDebuff();
    CheckSkill();
    CheckOutOfBound();

    HandleDamage();
    HandleDebuff(deltaTime);
    HandleSkill(deltaTime);
    HandlePlayerInput();
    HandlePlayerMovement(deltaTime);
    HandlePlayerAnimation(deltaTime);
    HandleGameOver(deltaTime, res, navigation);
    HandleGamePause(deltaTime, res, navigation);

    return res;
}

void GameMap::Mount(const std::any& args)
{
    gameFlags.gamePaused = false;
    LoadAudio();
    if (loadSave) {
        LoadGameData();
    } else {
        LoadSprites();
        character.Init(gameData.charaType, _CONSOLE_WIDTH_ / 2 - 32, 50);
        InitLaneList();
    }
    std::remove(SAVE_PATH);
    ChangeColorPalette(GetGamePalette(gameData.mapType, gameData.charaType));
}

void GameMap::Unmount()
{
    UnloadSprites();
    UnloadAudio();
}

void GameMap::HandlePlayerInput()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (IsKeyMeanEscape()) {
        gameFlags.gamePaused = true;
    } else {
        if (gameFlags.allowMovementKeys) {
            bool correctKeyFlag = !gameFlags.isReverseKey;
            if (IsKeyMeanUp()) {
                gameFlags.movingUp = correctKeyFlag;
                gameFlags.movingDown = !correctKeyFlag;
            }
            if (IsKeyMeanDown()) {
                gameFlags.movingDown = correctKeyFlag;
                gameFlags.movingUp = !correctKeyFlag;
            }
            if (IsKeyMeanLeft()) {
                gameFlags.movingLeft = correctKeyFlag;
                gameFlags.movingRight = !correctKeyFlag;
            }
            if (IsKeyMeanRight()) {
                gameFlags.movingRight = correctKeyFlag;
                gameFlags.movingLeft = !correctKeyFlag;
            }
        }
        if (IsKeyMeanSelect()) {
            gameFlags.skillCalled = true;
        }
    }
}

void GameMap::HandlePlayerAnimation(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (gameFlags.movingLeft) {
        character.SetSpriteLeft();
    } else if (gameFlags.movingRight) {
        character.SetSpriteRight();
    } else if (gameFlags.movingUp) {
        character.SetSpriteUp();
    } else if (gameFlags.movingDown) {
        character.SetSpriteDown();
    }

    if (gameFlags.isMoving) {
        if (gameFlags.justMoved) {
            character.AdvanceFrame();
        } else {
            character.UpdateFrame(deltaTime);
        }

    } else {
        character.ResetSprite();
    }
}

void GameMap::HandlePlayerMovement(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    bool moveFlag = false;
    float distanceY = character.getSpeed() * deltaTime;

    if (gameFlags.movingLeft && gameFlags.allowMoveLeft) {
        float distanceX;
        if (gameEventArgs.mapSpeedX < 0) {
            distanceX =
                deltaTime * (character.getSpeed() - gameEventArgs.mapSpeedX);
        } else {
            distanceX =
                deltaTime * (character.getSpeed() + gameEventArgs.mapSpeedX);
        }
        character.MoveLeft(distanceX);
        moveFlag = true;
    }
    if (gameFlags.movingRight && gameFlags.allowMoveRight) {
        float distanceX;
        if (gameEventArgs.mapSpeedX > 0) {
            distanceX =
                deltaTime * (character.getSpeed() + gameEventArgs.mapSpeedX);
        } else {
            distanceX =
                deltaTime * (character.getSpeed() - gameEventArgs.mapSpeedX);
        }
        character.MoveRight(distanceX);
        moveFlag = true;
    }
    if (gameFlags.movingUp && gameFlags.allowMoveUp) {
        character.MoveUp(distanceY);
        moveFlag = true;

        // score handle
        gameEventArgs.distWalked += distanceY;
        gameEventArgs.distWalkedSkill += distanceY;
        if (gameEventArgs.distWalked > MIN_DIST_SCORE) {
            gameEventArgs.distWalked = 0;
            gameEventArgs.currentScore += 1;
            if (gameEventArgs.currentScore % 50 == 0 && R.Config.Sfx) {
                gameAudio.scoreSfx.Play();
            }
        }
        if (gameEventArgs.distWalkedSkill > MIN_DIST_CHARGE) {
            gameEventArgs.distWalkedSkill = 0;
            if (gameEventArgs.skillCharge < MAX_SKILL_CHARGE) {
                gameEventArgs.skillCharge += 1;
                if (gameEventArgs.skillCharge == MAX_SKILL_CHARGE &&
                    R.Config.Sfx) {
                    gameAudio.skillReadySfx.Play();
                }
            }
        }
    }
    if (gameFlags.movingDown && gameFlags.allowMoveDown) {
        character.MoveDown(distanceY);
        moveFlag = true;

        gameEventArgs.distWalked -= distanceY;
        gameEventArgs.distWalkedSkill -= distanceY;
    }

    if (moveFlag) {
        if (gameFlags.isMoving == false) {
            gameFlags.justMoved = true;
        } else {
            gameFlags.justMoved = false;
        }
        gameFlags.isMoving = true;

    } else {
        gameFlags.isMoving = false;
    }
}

bool GameMap::HandleCharaDraw(
    ConsoleGame::AbstractCanvas* canvas, float charBottomY, float laneBottomY
) const
{
    if (gameFlags.isGameOver) return true;
    if (charBottomY > laneBottomY) {
        if (gameFlags.isDamageCooldown) {
            if (gameEventArgs.damageFlashingTimer >= 0.1) {
                character.Draw(canvas);
            }
        } else {
            character.Draw(canvas);
        }
        return true;
    }
    return false;
}

void GameMap::HandleItemCollision()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    switch (mapItem.GetType()) {
        case SPEED:
            character.SetSpeed(character.getSpeed() + SPEED_ADDITION);
            break;
        case STAR:
            gameEventArgs.skillCharge = 100;
            break;
        case HEALTH:
            int curHealth = character.GetCurHealth();
            if (curHealth < character.GetMaxHealth()) {
                character.SetCurHealth(curHealth + 1);
            }
            break;
    }
    gameEventArgs.numOfItemPick += 1;
}

void GameMap::HandleGameOver(
    float deltaTime,
    AbstractNavigation::NavigationRes& res,
    const AbstractNavigation* navigation
)
{
    if (!gameFlags.isGameOver) return;

    gameOverWait -= deltaTime;
    gameSprites.deathVfx.AutoUpdateFrame(deltaTime);
    if (gameOverWait <= 0) {
        //  go to next screen
        GameResult gameRes;
        gameRes.damage = gameEventArgs.damageTaken;
        gameRes.diff = gameEventArgs.difficultyReached;
        gameRes.numOfItem = gameEventArgs.numOfItemPick;
        gameRes.numOfMob = gameEventArgs.numOfMobsHit;
        gameRes.numOfSkill = gameEventArgs.numOfSkillUse;
        gameRes.score = gameEventArgs.currentScore;
        gameRes.time = gameEventArgs.playTime;
        gameRes.map = gameData.mapType;

        res = navigation->Navigate(L"Result", gameRes);
    }
}

void GameMap::HandleGamePause(
    float deltaTime,
    AbstractNavigation::NavigationRes& res,
    const ConsoleGame::AbstractNavigation* navigation
)
{
    if (!gameFlags.gamePaused) return;
    if (selectedScr == -1) {
        menu.Update(
            deltaTime,
            [&](uint8_t hovering) noexcept { audio.PlayHoverSfx(); },
            [&](uint8_t selected) noexcept {
                audio.PlayClickSfx();
                switch (selected) {
                    case 0:
                        gameFlags.gamePaused = false;
                        break;
                    case 1:
                        selectedScr = 0;
                        break;
                    case 2:
                        selectedScr = 1;
                        break;
                }
            }
        );
    } else {
        auto navRes = subScreen[selectedScr]->Update(deltaTime, navigation);
        if (navRes.ActionType == AbstractNavigation::NavigationAction::Back) {
            if (navRes.Payload.has_value()) {
                selectedScr = 0;
            } else {
                selectedScr = -1;
            }
        }
        if (navRes.ActionType ==
            AbstractNavigation::NavigationAction::Navigate) {
            if (navRes.ActionData == L"Yes") {
                res = navigation->PopBackTo(MainMenu::ScreenName());
                SaveGameData();
            } else if (navRes.ActionData == L"No") {
                res = navigation->PopBackTo(MainMenu::ScreenName());
            } else if (navRes.ActionData == HowToPlay::ScreenName()) {
                selectedScr = 2;
            } else if (navRes.ActionData == Credit::ScreenName()) {
                selectedScr = 3;
            }
        }
    }
}

void GameMap::TurnOffDebuff()
{
    gameEventArgs.mapDebuffTime = debuffDur[gameEventArgs.debuffType];
    gameEventArgs.mapDebuffCooldownTime = MAP_DEBUFF_COOLDOWN;
    if (gameFlags.debuffInUse) {
        switch (gameEventArgs.debuffType) {
            case FOREST:
                gameEventArgs.notMovingTime = 0;
                break;
            case CITY:
                character.SetCurHealth(gameEventArgs.originalHealth);
                gameEventArgs.originalHealth = 0;
                break;
            case HOUSE:
                gameFlags.isDarkMap = false;
                break;
            case DESERT:
                gameFlags.allowMovementKeys = true;
                gameFlags.allowSkillKey = true;
                break;
            case SPACE:
                gameFlags.allowSkill = true;
                break;
            case CASINO:
                gameFlags.isReverseKey = false;
                break;
        }
    }
    gameFlags.debuffInUse = false;
}

void GameMap::TurnOffSkill()
{
    gameFlags.turnOffSkill = false;
    if (!gameFlags.skillInUse) return;
    switch (gameEventArgs.skillType) {
        case FAUNA:
            character.SetMaxHealth(FAUNA_MAX_HEALTH);
            if (character.GetCurHealth() >= FAUNA_MAX_HEALTH) {
                character.SetCurHealth(FAUNA_MAX_HEALTH);
            }
            break;
        case IRYS:
            gameEventArgs.shield = 0;
            break;
        case MUMEI:
            character.SetSpeed(gameEventArgs.originalSpeed);
            gameFlags.isInvincible = false;
            break;
        case KRONII:
            gameFlags.allowLaneUpdate = true;
            gameEventArgs.mapDragSpeed = MAP_DRAG_SPEED;
            break;
        case SANA:
            gameFlags.allowDebuff = true;
            break;
        case BAE:
            gameFlags.isReverseKey = false;
            break;
    }
    gameFlags.skillInUse = false;
    gameFlags.allowSkill = true;
}

std::unique_ptr<Lane> GameMap::GetRandomLane()
{
    bool isLeftToRight = rand() % 2;

    // produce random lane
    float randNumForTrain = static_cast<float>(rand()) / RAND_MAX;
    LaneType laneType;
    if (randNumForTrain < RAIL_SPAWN_RATE) {
        laneType = RAIL;
    } else {
        laneType = static_cast<LaneType>(rand() % 3);
    }

    // produce random mob
    MobType mobType = static_cast<MobType>(rand() % gameEventArgs.mobRange);
    AniSprite* mobSprite = GetMobSprite(mobType, isLeftToRight);

    std::unique_ptr<Lane> lane;
    bool isNotSafeLane = true;

    switch (laneType) {
        case ROAD:
            lane = std::make_unique<Road>(
                laneList.back()->GetY() + 32,
                mobType,
                &gameSprites.roadSprite,
                mobSprite,
                isLeftToRight
            );
            break;
        case RAIL:
            lane = std::make_unique<Rail>(
                laneList.back()->GetY() + 32,
                mobType,
                &gameSprites.roadSprite,
                mobSprite,
                isLeftToRight
            );
            break;
        case SAFE:
            lane = std::make_unique<SafeZone>(
                laneList.back()->GetY() + 32,
                &gameSprites.safeSprite,
                &gameSprites.blockSprite,
                isLeftToRight
            );
            isNotSafeLane = false;
            break;
        case WATER:
            lane = std::make_unique<Water>(
                laneList.back()->GetY() + 32,
                &gameSprites.waterSprite,
                &gameSprites.floatSprite,
                isLeftToRight
            );
            break;
    }

    // item handle
    if (!gameFlags.mapHasItem && isNotSafeLane) {
        float randomNum = static_cast<float>(rand()) / RAND_MAX;
        if (randomNum < ITEM_SPAWN_RATE) {
            lane->SetHasItem(true);
            laneWithItem = lane.get();
            ItemType itemType = static_cast<ItemType>((int)(rand() % 3));
            itemType = HEALTH;
            AniSprite* itemSprite = &gameSprites.itemSpeed;
            switch (itemType) {
                case SPEED:
                    itemSprite = &gameSprites.itemSpeed;
                    break;
                case STAR:
                    itemSprite = &gameSprites.itemStar;
                    break;
                case HEALTH:
                    itemSprite = &gameSprites.itemHealth;
                    break;
            }
            mapItem.Init(
                rand() % (_CONSOLE_WIDTH_ - 64) + 64,
                lane->GetY(),
                itemType,
                itemSprite
            );
            gameFlags.mapHasItem = true;
        }
    }
    return lane;
}

ConsoleGame::AniSprite* GameMap::GetMobSprite(
    GameType::MobType type, bool isLeftToRight
)
{
    if (isLeftToRight) {
        switch (type) {
            case EASY:
                return &gameSprites.mobSpriteEasy.MobRight;
                break;
            case NORMAL:
                return &gameSprites.mobSpriteNormal.MobRight;
                break;
            case HARD:
                return &gameSprites.mobSpriteHard.MobRight;
                break;
        }
    } else {
        switch (type) {
            case EASY:
                return &gameSprites.mobSpriteEasy.MobLeft;
                break;
            case NORMAL:
                return &gameSprites.mobSpriteNormal.MobLeft;
                break;
            case HARD:
                return &gameSprites.mobSpriteHard.MobLeft;
                break;
        }
    }
}

ConsoleGame::AniSprite& GameMap::GetItemSprite(GameType::ItemType type)
{
    switch (type) {
        case SPEED:
            return gameSprites.itemSpeed;
            break;
        case STAR:
            return gameSprites.itemStar;
            break;
        case HEALTH:
            return gameSprites.itemHealth;
            break;
    }
}

void GameMap::ResiseBlockHitBox()
{
    Box hitbox = gameSprites.blockSprite.GetHitBox();
    if (hitbox.dim.height <= 32) return;
    int yOffset = hitbox.dim.height - 32;
    int newHeight = 32;
    Vec2 coordOffSet = {.x = 0, .y = yOffset};
    Vec2 dimOffset = {.width = 0, .height = -(hitbox.dim.height - 32)};
    gameSprites.blockSprite.EditHitBox(coordOffSet, dimOffset);
}

void GameMap::LoadSprites()
{
    // mob sprites
    LoadMobSprite(gameData.mapType, MobType::EASY, gameSprites.mobSpriteEasy);
    LoadMobSprite(
        gameData.mapType, MobType::NORMAL, gameSprites.mobSpriteNormal
    );
    LoadMobSprite(gameData.mapType, MobType::HARD, gameSprites.mobSpriteHard);

    // static sprites
    LoadMapSprite(gameData.mapType, gameSprites.blockSprite, "block");
    LoadMapSprite(gameData.mapType, gameSprites.floatSprite, "float");
    LoadMapSprite(gameData.mapType, gameSprites.roadSprite, "road");
    LoadMapSprite(gameData.mapType, gameSprites.safeSprite, "safe");
    LoadMapSprite(gameData.mapType, gameSprites.waterSprite, "water");
    LoadMapSprite(gameData.mapType, gameSprites.debuff, "debuff");

    LoadExtraSprite(gameSprites.emptyHealth, "health-empty");
    LoadCharaSprite(gameData.charaType, gameSprites.health, "health");
    LoadCharaSprite(gameData.charaType, gameSprites.skill, "skill");

    // items
    gameSprites.itemSpeed.Load(RESOURCE_PATH EXTRA_PATH "item-speed.anisprite");
    LoadHeartSprite(gameSprites.itemHealth, gameData.charaType);
    gameSprites.itemStar.Load(RESOURCE_PATH EXTRA_PATH "item-star.anisprite");

    float frameDur = 0.15f;
    gameSprites.itemSpeed.SetFrameDuration(frameDur);
    gameSprites.itemHealth.SetFrameDuration(frameDur);
    gameSprites.itemStar.SetFrameDuration(frameDur);

    gameSprites.itemSpeed.Play(1);
    gameSprites.itemHealth.Play(1);
    gameSprites.itemStar.Play(1);

    gameSprites.mobSpriteEasy.MobLeft.Play(1);
    gameSprites.mobSpriteEasy.MobRight.Play(1);
    gameSprites.mobSpriteNormal.MobLeft.Play(1);
    gameSprites.mobSpriteNormal.MobRight.Play(1);
    gameSprites.mobSpriteHard.MobLeft.Play(1);
    gameSprites.mobSpriteHard.MobRight.Play(1);

    // effects
    gameSprites.deathVfx.Load(RESOURCE_PATH EXTRA_PATH "death.anisprite");
    gameSprites.deathVfx.Play();

    ResiseBlockHitBox();
}

void GameMap::LoadAudio()
{
    gameAudio.itemPickSfx.Open(RESOURCE_PATH SFX_PATH "item-pick.wav");
    gameAudio.damageSfx.Open(RESOURCE_PATH SFX_PATH "hurt.wav");
    gameAudio.deadSfx.Open(RESOURCE_PATH SFX_PATH "dead.wav");
    gameAudio.warningSfx.Open(RESOURCE_PATH SFX_PATH "warning.wav");
    gameAudio.shieldBreakSfx.Open(RESOURCE_PATH SFX_PATH "shield-break.wav");
    gameAudio.scoreSfx.Open(RESOURCE_PATH SFX_PATH "score.wav");

    gameAudio.skillReadySfx.Open(RESOURCE_PATH SFX_PATH "skill-ready.wav");
    gameAudio.skillOverSfx.Open(RESOURCE_PATH SFX_PATH "skill-end.wav");

    gameAudio.debuffActivateSfx.Open(RESOURCE_PATH SFX_PATH "debuff-start.wav");
    gameAudio.debuffOverSfx.Open(RESOURCE_PATH SFX_PATH "debuff-over.wav");

    gameAudio.skillFaunaSfx.Open(RESOURCE_PATH SFX_PATH "fauna-skill.wav");
    gameAudio.skillIrysSfx.Open(RESOURCE_PATH SFX_PATH "irys-skill.wav");
    gameAudio.skillMumeiSfx.Open(RESOURCE_PATH SFX_PATH "mumei-skill.wav");
    gameAudio.skillKroniiSfx.Open(RESOURCE_PATH SFX_PATH "kronii-skill.wav");
    gameAudio.skillSanaSfx.Open(RESOURCE_PATH SFX_PATH "sana-skill.wav");
    gameAudio.skillBaeSfx.Open(RESOURCE_PATH SFX_PATH "bae-skill.wav");
}

void GameMap::UnloadSprites()
{
    // static
    gameSprites.floatSprite.Unload();
    gameSprites.blockSprite.Unload();
    gameSprites.health.Unload();
    gameSprites.emptyHealth.Unload();

    gameSprites.itemHealth.Unload();
    gameSprites.itemSpeed.Unload();
    gameSprites.itemStar.Unload();

    gameSprites.skill.Unload();
    gameSprites.debuff.Unload();

    // mob
    gameSprites.mobSpriteEasy.MobLeft.Unload();
    gameSprites.mobSpriteEasy.MobRight.Unload();

    gameSprites.mobSpriteNormal.MobLeft.Unload();
    gameSprites.mobSpriteNormal.MobRight.Unload();

    gameSprites.mobSpriteHard.MobLeft.Unload();
    gameSprites.mobSpriteHard.MobRight.Unload();

    // lane
    gameSprites.roadSprite.Unload();
    gameSprites.waterSprite.Unload();
    gameSprites.safeSprite.Unload();

    // effects
    gameSprites.deathVfx.Unload();

    // chara
    character.UnloadSprites();
}

void GameMap::UnloadAudio()
{
    gameAudio.damageSfx.Close();
    gameAudio.deadSfx.Close();
    gameAudio.warningSfx.Close();
    gameAudio.debuffActivateSfx.Close();
    gameAudio.debuffOverSfx.Close();
    gameAudio.itemPickSfx.Close();
    gameAudio.scoreSfx.Close();
    gameAudio.shieldBreakSfx.Close();

    gameAudio.skillBaeSfx.Close();
    gameAudio.skillFaunaSfx.Close();
    gameAudio.skillIrysSfx.Close();
    gameAudio.skillMumeiSfx.Close();
    gameAudio.skillKroniiSfx.Close();
    gameAudio.skillSanaSfx.Close();
    gameAudio.skillOverSfx.Close();
    gameAudio.skillReadySfx.Close();
};

void GameMap::SetGameMapData(const GameMapData& gmData)
{
    gameData.mapType = gmData.mapType;
    gameData.charaType = gmData.charaType;
    gameData.mapMode = gmData.mapMode;
    gameData.mapDifficulty = gmData.mapDifficulty;
}

void GameMap::Draw(AbstractCanvas* canvas) const
{
    DrawFlat(canvas);
    DrawEntity(canvas);
    DrawDarkness(canvas);
    DrawHealth(canvas);
    DrawSkill(canvas);
    DrawDebuff(canvas);
    DrawScore(canvas);
    DrawDeathVFX(canvas);
    DrawTime(canvas);
    if (gameFlags.gamePaused) {
        if (selectedScr == -1) {
            menu.Draw(canvas);
        } else {
            subScreen[selectedScr]->Draw(canvas);
        }
    }
}

void GameMap::DrawFlat(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.end();
    int screenHeight = _CONSOLE_HEIGHT_ * 2 + 32;
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() <= screenHeight) {
            lane->DrawLane(canvas);
        }
    }
}

void GameMap::DrawEntity(ConsoleGame::AbstractCanvas* canvas) const
{
    auto laneListEnd = laneList.rend();
    bool charaDrawn = false;
    float charBottomY = character.GetBottomY();
    int screenHeight = _CONSOLE_HEIGHT_ * 2 + 32;

    bool itemDrawn = false;

    for (auto it = laneList.rbegin(); it != laneListEnd; ++it) {
        Lane* lane = it->get();
        if (lane->GetY() <= screenHeight) {
            if (!charaDrawn && lane->GetType() != WATER) {
                charaDrawn =
                    HandleCharaDraw(canvas, charBottomY, lane->GetBottomY());
            }
            lane->DrawEntity(canvas);
            if (gameFlags.mapHasItem && lane->GetHasItem()) {
                mapItem.Draw(canvas);
                itemDrawn = true;
            }
        }
    }
    if (!charaDrawn) {
        character.Draw(canvas);
    }
}

void GameMap::DrawHealth(AbstractCanvas* canvas) const
{
    Vec2 coord{.x = 5, .y = 4};
    int maxHealth = character.GetMaxHealth();
    int curHealth = character.GetCurHealth();
    int margin = 0;
    int space = 9 + margin;
    for (int i = 0; i < maxHealth; ++i) {
        gameSprites.emptyHealth.Draw(canvas, coord);
        coord.x += space;
    }
    coord.x = 5;
    for (int i = 0; i < curHealth; ++i) {
        gameSprites.health.Draw(canvas, coord);
        coord.x += space;
    }
}

void GameMap::DrawSkill(ConsoleGame::AbstractCanvas* canvas) const
{
    int skillX = 3;
    int skillY = 14;

    // skill icon
    Vec2 skillCoord{.x = skillX, .y = skillY};
    gameSprites.skill.Draw(canvas, skillCoord);

    // skill charge
    std::string percentStr =
        std::to_string((int)gameEventArgs.skillCharge) + "%";
    Vec2 dim = Font::GetDim(1);
    Vec2 percentCoord = {.x = skillX + 17, .y = skillY + 2};
    Font::DrawString(canvas, percentStr, percentCoord, 1, 1, (Color)14);

    // active skill icon
    if (gameFlags.skillInUse) {
        int activeX = ConsoleGame::_CONSOLE_WIDTH_ - 19;
        int activeY = 40;
        Vec2 activeSkillCoord{.x = activeX, .y = activeY};
        for (size_t i = 0; i < 15; ++i) {
            for (size_t j = 0; j < 15; ++j) {
                (*canvas)[activeY + i][activeX + j] = (Color)1;
            }
        }
        gameSprites.skill.Draw(canvas, activeSkillCoord);

        for (size_t j = 0; j < 15; ++j) {
            (*canvas)[activeY][activeX + j] = (Color)14;
            (*canvas)[activeY + 15][activeX + j] = (Color)14;
        }
        for (size_t i = 0; i < 16; ++i) {
            (*canvas)[activeY + i][activeX] = (Color)14;
            (*canvas)[activeY + i][activeX + 15] = (Color)14;
        }

        if (gameEventArgs.skillCategory == SHIELD) {
            std::string shieldStr = std::to_string((int)gameEventArgs.shield);
            Font::DrawString(
                canvas,
                shieldStr,
                {.x = activeSkillCoord.x + 4, .y = activeSkillCoord.y + 2},
                1,
                1,
                (Color)14
            );
        }
    }
}

void GameMap::DrawDebuff(ConsoleGame::AbstractCanvas* canvas) const
{
    if (!gameFlags.debuffWarning && !gameFlags.debuffInUse) return;
    Vec2 coord{.x = ConsoleGame::_CONSOLE_WIDTH_ - 19, .y = 18};
    if (gameFlags.debuffWarning) {
        if (gameEventArgs.debuffFlasingTimer >= 0.5) {
            gameSprites.debuff.Draw(canvas, coord);
        }
    } else {
        gameSprites.debuff.Draw(canvas, coord);
    }
}

void GameMap::DrawDarkness(ConsoleGame::AbstractCanvas* canvas) const
{
    if (!gameFlags.isDarkMap) return;
    Box charaBox = character.GetHitBox();
    int screenWidth = _CONSOLE_WIDTH_;
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    int xCenter = charaBox.coord.x + charaBox.dim.width / 2;
    int yCenter = screenHeight - charaBox.coord.y;

    int visibleRadius = VISIBLE_RADIUS;
    Color darknessColor = Color(13);
    for (int y = 0; y <= screenHeight; ++y) {
        for (int x = 0; x <= screenWidth; ++x) {
            if (GetDistance(xCenter, yCenter, x, y) > visibleRadius) {
                (*canvas)[y][x] = darknessColor;
            }
        }
    }
}

void GameMap::DrawScore(ConsoleGame::AbstractCanvas* canvas) const
{
    std::string scoreString = std::to_string(gameEventArgs.currentScore);
    Vec2 dim = Font::GetDim(1);
    int pushBack = scoreString.length() * dim.width + 3;
    Vec2 drawCoord = {.x = _CONSOLE_WIDTH_ - pushBack, .y = 5};
    Font::DrawString(canvas, scoreString, drawCoord, 1, 1, (Color)14);
}

void GameMap::DrawDeathVFX(ConsoleGame::AbstractCanvas* canvas) const
{
    if (!gameFlags.isGameOver) return;
    if (gameOverWait <= 2.5) return;
    Box hitbox = character.GetHitBox();
    float charaY = hitbox.coord.y + hitbox.dim.height / 2;
    float charaX = character.GetX() + hitbox.dim.width / 2;
    int screenHeight = _CONSOLE_HEIGHT_ * 2;
    gameSprites.deathVfx.Draw(
        canvas, {.x = (int)charaX, .y = screenHeight - (int)charaY}
    );
}

void GameMap::DrawTime(ConsoleGame::AbstractCanvas* canvas) const
{
    if (gameData.mapMode != NINF) return;
    Font::DrawString(
        canvas, SecondsToMMSS(gameEventArgs.timeLeft), {5, 34}, 1, 1, (Color)14
    );
}

void GameMap::ResetFlags()
{
    if (gameFlags.gamePaused) return;
    gameFlags.allowMoveRight = true;
    gameFlags.allowMoveLeft = true;
    gameFlags.allowMoveUp = true;
    gameFlags.allowMoveDown = true;

    gameFlags.movingLeft = false;
    gameFlags.movingRight = false;
    gameFlags.movingUp = false;
    gameFlags.movingDown = false;

    gameFlags.blockCollision = false;
    gameFlags.damageCollision = false;
    gameFlags.logCollision = false;
    gameFlags.itemCollision = false;
}

void GameMap::CheckCollision(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    // lane collision
    size_t listSize = laneList.size();
    for (int i = 0; i < listSize; ++i) {
        if (laneList[i]->ContainsChara(character)) {
            CheckCollisionAgain(laneList[i].get(), deltaTime);
            if (i - 1 >= 0) {
                CheckCollisionAgain(laneList[i - 1].get(), deltaTime);
            }
            if (i + 1 < listSize) {
                CheckCollisionAgain(laneList[i + 1].get(), deltaTime);
            }
        }
    }

    // item collision
    if (gameFlags.mapHasItem) {
        Box charaHitbox = character.GetHitBox();
        Box itemHitbox = mapItem.GetHitBox();
        CollisionType colType = GetCollisionType(charaHitbox, itemHitbox);
        if (colType != CollisionType::None) {
            HandleItemCollision();
            if (R.Config.Sfx) {
                gameAudio.itemPickSfx.Play();
            }
            laneWithItem->SetHasItem(false);
            gameFlags.mapHasItem = false;
        }
    }
}

void GameMap::CheckDebuff()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    if (!gameFlags.debuffCalled) return;
    if (!gameFlags.allowDebuff) return;

    gameFlags.debuffCalled = false;

    MapType mapType = gameData.mapType;
    if (mapType == CASINO) {
        int randInd = std::rand() % (CASINO + 1);
        mapType = static_cast<MapType>(randInd);
    }
    gameEventArgs.debuffType = mapType;
    gameEventArgs.mapDebuffTime = debuffDur[mapType];
    gameFlags.debuffInUse = true;

    if (R.Config.Sfx) {
        gameAudio.debuffActivateSfx.Play();
    }
}

void GameMap::CheckSkill()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (!gameFlags.skillCalled) return;
    gameFlags.skillCalled = false;
    if (!gameFlags.allowSkill) return;
    if (gameEventArgs.skillCharge < MAX_SKILL_CHARGE) return;

    CharaType charaType = gameData.charaType;
    if (charaType == BAE) {
        int randInd = std::rand() % (BAE + 1);
        charaType = static_cast<CharaType>(randInd);
    }

    if (R.Config.Sfx) {
        switch (charaType) {
            case FAUNA:
                gameAudio.skillFaunaSfx.Play();
                break;
            case IRYS:
                gameAudio.skillIrysSfx.Play();
                break;
            case MUMEI:
                gameAudio.skillMumeiSfx.Play();
                break;
            case KRONII:
                gameAudio.skillKroniiSfx.Play();
                break;
            case SANA:
                gameAudio.skillSanaSfx.Play();
                break;
            case BAE:
                gameAudio.skillBaeSfx.Play();
                break;
        }
    }

    gameEventArgs.skillType = charaType;
    gameFlags.skillActivate = true;
    gameEventArgs.skillCharge = 0;
    gameEventArgs.numOfSkillUse += 1;
}

void GameMap::CheckOutOfBound()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    float charaX = character.GetX();
    float charaY = character.GetY();
    float charaWidth = character.GetHitBox().dim.width;
    if (charaX > _CONSOLE_WIDTH_ || charaY < 0 || charaX + charaWidth < 0) {
        character.SetCurHealth(0);
    }
}

void GameMap::CheckGameOver()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (character.GetCurHealth() <= 0) {
        gameFlags.isGameOver = true;
        if (R.Config.Sfx) {
            gameAudio.deadSfx.Play();
        }
    }
}

void GameMap::CheckCollisionAgain(Lane* lane, float deltaTime)
{
    LaneType laneType = lane->GetType();
    if (laneType == LaneType::WATER) {
        CollisionType waterColType = lane->GetLaneCollision(character);
        if (waterColType != CollisionType::Bottom &&
            waterColType != CollisionType::Top) {
            if (lane->ContainsChara(character)) {
                HandleCharaOnLog(lane, deltaTime);
            }
        }
        HandleWaterCollision(waterColType);

    } else if (laneType == LaneType::ROAD || laneType == LaneType::RAIL) {
        if (lane->ContainsChara(character)) {
            CollisionType colType = lane->GetCollision(character);
            if (colType != CollisionType::None) {
                HandleCollision(lane, colType);
            }
        }
    } else {
        CollisionType colType = lane->GetCollision(character);
        if (colType != CollisionType::None) {
            HandleCollision(lane, colType);
        }
    }
}

void GameMap::UpdateLanes(float deltaTime)
{
    if (gameFlags.gamePaused) return;

    if (!gameFlags.allowLaneUpdate) return;
    for (auto& lane : laneList) {
        LaneType type = lane->GetType();
        if (type == LaneType::SAFE) {
            continue;
        }
        lane->UpdatePos(deltaTime);
    }
}

void GameMap::HandleCollision(Lane* lane, CollisionType colType)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    Road* road = nullptr;
    Rail* rail = nullptr;
    switch (lane->GetType()) {
        case ROAD:
            road = dynamic_cast<Road*>(lane);
            gameEventArgs.collidedMobtype = road->GetMobType();
            gameFlags.damageCollision = true;
            break;
        case RAIL:
            rail = dynamic_cast<Rail*>(lane);
            gameEventArgs.collidedMobtype = rail->GetMobType();
            gameFlags.damageCollision = true;
            break;
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
        case WATER:
            break;
    }
}

void GameMap::HandleWaterCollision(GameType::CollisionType colType)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (colType == CollisionType::Left) {
        gameFlags.allowMoveRight = false;
    } else if (colType == CollisionType::Right) {
        gameFlags.allowMoveLeft = false;
    } else if (colType == CollisionType::Top) {
        gameFlags.allowMoveDown = false;
    } else if (colType == CollisionType::Bottom) {
        gameFlags.allowMoveUp = false;
    }
}

void GameMap::HandleCharaOnLog(Lane* lane, float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    if (!gameFlags.allowLaneUpdate) return;
    float laneSpeed = lane->GetSpeed();
    float distance = deltaTime * laneSpeed;
    if (lane->GetIsLeftToRight()) {
        character.MoveRight(distance);
        gameEventArgs.mapSpeedX = laneSpeed;
    } else {
        character.MoveLeft(distance);
        gameEventArgs.mapSpeedX = -laneSpeed;
    }
}

void GameMap::HandleDamage()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (gameFlags.isDamageCooldown || !gameFlags.damageCollision ||
        gameFlags.isInvincible)
        return;
    if (gameEventArgs.shield > 0) {
        --gameEventArgs.shield;
        if (gameEventArgs.shield > 0 && R.Config.Sfx) {
            gameAudio.shieldBreakSfx.Play();
        }

    } else {
        int newHealth =
            character.GetCurHealth() - (gameEventArgs.collidedMobtype + 1);
        gameEventArgs.damageTaken += gameEventArgs.collidedMobtype + 1;
        character.SetCurHealth(newHealth);

        if (character.GetCurHealth() > 0 && R.Config.Sfx) {
            gameAudio.damageSfx.Play();
        }
    }
    gameFlags.isDamageCooldown = true;
    gameEventArgs.damageCooldownTime = 3;
    gameEventArgs.numOfMobsHit += 1;
}

void GameMap::HandleDebuff(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    if (!gameFlags.debuffInUse) return;

    int curHealth = character.GetCurHealth();
    switch (gameEventArgs.debuffType) {
        case FOREST:
            if (gameFlags.isMoving == false) {
                gameEventArgs.notMovingTime += deltaTime;
            } else {
                gameEventArgs.notMovingTime = 0;
            }
            if (gameEventArgs.notMovingTime >= MAX_IDLE_TIME) {
                int newHealth = character.GetCurHealth() - 1;
                character.SetCurHealth(newHealth);

                gameEventArgs.damageTaken += 1;
                gameEventArgs.notMovingTime = 0;
                gameFlags.isDamageCooldown = true;
                gameEventArgs.damageCooldownTime = 3;

                if (R.Config.Sfx) {
                    gameAudio.damageSfx.Play();
                }
            }
            break;
        case CITY:
            if (curHealth > IRYS_DEBUFF_HEALTH) {
                gameEventArgs.originalHealth = curHealth;
                character.SetCurHealth(IRYS_DEBUFF_HEALTH);
            }
            break;
        case HOUSE:
            gameFlags.isDarkMap = true;
            break;
        case DESERT:
            gameFlags.allowMovementKeys = false;
            if (gameData.charaType != SANA) {
                gameFlags.allowSkillKey = false;
            }
            break;
        case SPACE:
            TurnOffSkill();
            gameFlags.allowSkill = false;
            break;
        case CASINO:
            gameFlags.isReverseKey = true;
            break;
    }

    gameEventArgs.mapDebuffTime -= deltaTime;
    if (gameEventArgs.mapDebuffTime <= 0) {
        if (R.Config.Sfx) {
            gameAudio.debuffOverSfx.Play();
        }
        TurnOffDebuff();
    }
}

void GameMap::HandleSkill(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (gameFlags.skillActivate) {
        gameEventArgs.skillTime = SKILL_DURATION;
        int curSpeed = character.getSpeed();
        switch (gameEventArgs.skillType) {
            case FAUNA:
                character.SetMaxHealth(FAUNA_EXTRA_MAX_HEALTH);
                character.SetCurHealth(FAUNA_EXTRA_MAX_HEALTH);
                gameEventArgs.skillCategory = TIME;
                break;
            case IRYS:
                gameEventArgs.shield = IRYS_SHIELD_COUNT;
                gameEventArgs.skillCategory = SHIELD;
                break;
            case MUMEI:
                gameEventArgs.originalSpeed = curSpeed;
                character.SetSpeed(curSpeed + MUMEI_SPEED_BUFF);
                gameFlags.isInvincible = true;
                gameEventArgs.skillCategory = TIME;
                break;
            case KRONII:
                gameFlags.allowLaneUpdate = false;
                gameEventArgs.mapDragSpeed = 0;
                gameEventArgs.skillCategory = TIME;
                break;
            case SANA:
                if (gameFlags.debuffInUse) {
                    TurnOffDebuff();
                }
                gameEventArgs.skillCategory = TIME;
                gameFlags.allowDebuff = false;
                break;
            case BAE:
                gameFlags.isReverseKey = true;
                gameEventArgs.skillCategory = TIME;
                break;
        }
        gameFlags.skillActivate = false;
        gameFlags.skillInUse = true;
        gameFlags.allowSkill = false;
    }
    if (gameFlags.skillInUse) {
        switch (gameEventArgs.skillCategory) {
            case TIME:
                gameEventArgs.skillTime -= deltaTime;
                if (gameEventArgs.skillTime <= 0) {
                    gameFlags.turnOffSkill = true;
                }
                break;
            case SHIELD:
                if (gameEventArgs.shield == 0) {
                    gameFlags.turnOffSkill = true;
                }
                break;
        }
    }

    if (gameFlags.turnOffSkill) {
        if (R.Config.Sfx) {
            gameAudio.skillOverSfx.Play();
        }
        TurnOffSkill();
    }
}

void GameMap::UpdateCooldowns(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (gameFlags.isDamageCooldown) {
        gameEventArgs.damageCooldownTime -= deltaTime;
        if (gameEventArgs.damageFlashingTimer >= 0.2) {
            gameEventArgs.damageFlashingTimer = 0;
        }
        gameEventArgs.damageFlashingTimer += deltaTime;
    }

    if (!gameFlags.debuffInUse) {
        gameEventArgs.mapDebuffCooldownTime -= deltaTime;
        if (gameEventArgs.mapDebuffCooldownTime <= 4 &&
            !gameFlags.debuffWarning) {
            gameFlags.debuffWarning = true;

            if (R.Config.Sfx) {
                gameAudio.warningSfx.Play();
            }
        }
        if (gameFlags.debuffWarning) {
            if (gameEventArgs.debuffFlasingTimer >= 1) {
                gameEventArgs.debuffFlasingTimer = 0;
            }
            gameEventArgs.debuffFlasingTimer += deltaTime;
        }
        if (gameEventArgs.mapDebuffCooldownTime <= 0) {
            if (gameFlags.allowDebuff) {
                gameFlags.debuffCalled = true;
            } else {
                TurnOffDebuff();
            }
            gameFlags.debuffWarning = false;
        }
    }

    if (gameEventArgs.damageCooldownTime <= 0) {
        gameFlags.isDamageCooldown = false;
    }
}

void GameMap::UpdateDifficulty()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    if (gameData.mapDifficulty != MPROG) return;

    if (gameEventArgs.currentScore >= 150) {
        gameEventArgs.mobRange = 3;
        gameEventArgs.difficultyReached = HARD;
        if (gameFlags.allowLaneUpdate && !gameFlags.skillInUse) {
            gameEventArgs.mapDragSpeed = MAP_DRAG_SPEED;
        }
    } else if (gameEventArgs.currentScore >= 50) {
        gameEventArgs.mobRange = 2;
        gameEventArgs.mapDragSpeed = 0;
        gameEventArgs.difficultyReached = NORMAL;
    } else {
        gameEventArgs.mobRange = 1;
        gameEventArgs.mapDragSpeed = 0;
        gameEventArgs.difficultyReached = EASY;
    }
}

void GameMap::UpdateMapSpeed()
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;

    if (character.GetY() >= _CONSOLE_HEIGHT_) {
        gameEventArgs.mapSpeedY = character.getSpeed();
    } else {
        gameEventArgs.mapSpeedY = gameEventArgs.mapDragSpeed;
    }
}

void GameMap::UpdateTime(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    gameEventArgs.playTime += deltaTime;

    if (gameData.mapMode != NINF) return;
    gameEventArgs.timeLeft -= deltaTime;
    if (gameEventArgs.timeLeft <= 0) {
        gameFlags.isGameOver = true;
        if (R.Config.Sfx) {
            gameAudio.deadSfx.Play();
        }
    }
}

void GameMap::UpdateSprites(float deltaTime)
{
    if (gameFlags.gamePaused) return;
    if (!gameFlags.allowLaneUpdate) return;

    gameSprites.mobSpriteEasy.MobLeft.AutoUpdateFrame(deltaTime);
    gameSprites.mobSpriteEasy.MobRight.AutoUpdateFrame(deltaTime);
    gameSprites.mobSpriteNormal.MobLeft.AutoUpdateFrame(deltaTime);
    gameSprites.mobSpriteNormal.MobRight.AutoUpdateFrame(deltaTime);
    gameSprites.mobSpriteHard.MobLeft.AutoUpdateFrame(deltaTime);
    gameSprites.mobSpriteHard.MobRight.AutoUpdateFrame(deltaTime);

    if (gameFlags.mapHasItem) {
        mapItem.UpdateSprite(deltaTime);
    }
}

void GameMap::DragMapDown(float deltatime)
{
    if (gameFlags.gamePaused) return;
    if (gameFlags.isGameOver) return;
    float distance = deltatime * gameEventArgs.mapSpeedY;
    character.MoveDown(distance);

    auto laneListEnd = laneList.end();
    for (auto it = laneList.begin(); it != laneListEnd; ++it) {
        auto lane = it->get();
        lane->SetY(lane->GetY() - distance);
    }

    if (gameFlags.mapHasItem) {
        mapItem.SetY(mapItem.GetY() - distance);
    }

    if (!laneList.empty()) {
        std::unique_ptr<Lane>& frontLane = laneList.front();
        float roadTopY = frontLane->GetTopY();
        if (roadTopY < -32) {
            if (frontLane->GetHasItem()) {
                gameFlags.mapHasItem = false;
            }
            laneList.erase(laneList.begin());
            laneList.push_back(GetRandomLane());
        }
    }
}
