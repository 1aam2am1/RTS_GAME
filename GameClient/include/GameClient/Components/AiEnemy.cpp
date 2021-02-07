//
// Created by Michal_Marszalek on 2021-01-22.
//

#include <Core/Application.h>
#include "AiEnemy.h"
#include "ResourceShip.h"
#include "AttackShip.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Core/Time.h>
#include <GameClient/Components/Life/Life.h>
#include <GameClient/MainThread.h>

ADD_USER_COMPONENT(AiEnemy, base, time_to_decision, depth)

struct AiEnemy::Data {
    struct {
        float resources[3];
        int resource_building;
        int attack_building;

        int resource_ship;
        int attack_ship;

        std::array<float, 24> time_to_resource_ship;
        std::array<float, 24> time_to_attack_ship;

        float ship_life;
        float buildings_life;

        float base_life;

        bool user_attacks;
    } user[2];
};

void AiEnemy::Update() {
    time_to_decision -= Time::deltaTime();

    if (!base || !base->other_enemy) { return; }
    if (thread_runs) { return; }

    if (time_to_decision <= 0) {
        Data data;
        {
            auto &user = data.user[false];
            auto &ai = data.user[true];

            auto write = [](auto &data, TPtr<Enemy> &enemy) {
                data.resources[(int) ResourceType::water] = enemy->resources[ResourceType::water];
                data.resources[(int) ResourceType::metal] = enemy->resources[ResourceType::metal];
                data.resources[(int) ResourceType::food] = enemy->resources[ResourceType::food];

                data.resource_building = 0;
                data.attack_building = 0;
                for (auto &b : enemy->buildings) {
                    switch (b->type) {
                        case ShipType::Resource:
                            data.resource_building += 1;
                            break;
                        case ShipType::Attack:
                            data.attack_building += 1;
                            break;
                    }
                }
                data.resource_ship = 0;
                data.attack_ship = 0;
                for (auto &b : enemy->ships) {
                    if (dynamic_cast<AttackShip *>(b.get())) {
                        data.attack_ship++;
                    } else if (dynamic_cast<ResourceShip *>(b.get())) {
                        data.resource_ship++;
                    }
                }

                for (auto &t2 : data.time_to_resource_ship) {
                    t2 = 0;
                }
                for (auto &t2 : data.time_to_attack_ship) {
                    t2 = 0;
                }

                data.ship_life = 0;
                data.buildings_life = 0;

                auto life = enemy->template GetComponent<Life>();
                data.base_life = life ? life->life : 1;

                data.user_attacks = enemy->cell == mono_state::attack;
            };

            write(user, base->other_enemy);
            write(ai, base);
        }

        thread_runs = true;
        if (t.joinable()) { t.detach(); }
        t = std::thread([&thread_runs = this->thread_runs, data, depth = this->depth, base = this->base]() {
            auto vec = actions(data, true);

            Actions action = Wait;

            auto alfa = std::numeric_limits<float>::lowest();
            auto beta = std::numeric_limits<float>::max();
            for (size_t i = 0; i < vec.second; ++i) {
                auto d = simulate(data, vec.first[i], true, 10);
                auto val = min_max(d, depth, true,
                                   0, 0,
                                   alfa, beta);

                if (val > alfa) {
                    alfa = val;
                    action = vec.first[i];
                }
            }
            if (base) {
                MainThread::Invoke([base, action]() {
                    if (!base) { return; }
                    //Make this in main thread!!!
                    if (base->cell == mono_state::attack && action == Wait) {
                        base->cell = mono_state::flee;
                    }

                    base->cell = mono_state::wait;
                    switch (action) {
                        case Attack: {
                            base->cell = mono_state::attack;
                        }
                            break;
                        case Wait: {
                            base->cell = mono_state::resource;
                        }
                            break;
                        case Build_attack_building: {
                            //TODO:!!!
                            GameApi::log(ERR << "To do");
                        }
                            break;
                        case Build_resource_building: {
                            //TODO:!!!
                            GameApi::log(ERR << "To do");
                        }
                            break;
                        case Build_attack_ship: {
                            base->ProduceShip(ShipType::Attack);
                        }
                            break;
                        case Build_resource_ship: {
                            base->ProduceShip(ShipType::Resource);
                        }
                    }
                });
            }

            thread_runs = false;
        });

        time_to_decision = 1;
    }
}

float AiEnemy::min_max(const Data &data, int depth, bool max, float tA, float tB, float alfa, float beta) {
    if (depth <= 0) {
        return calculate(data);
    }

    auto t = std::min(tA, tB);
    t = t > 20 ? 20 : t;

    auto vec = actions(data, max);

    for (size_t i = 0; i < vec.second; ++i) {
        auto d = simulate(data, vec.first[i], max, t);
        tA = NextActionTime(d, max);

        float val = min_max(d, depth - 1, !max, tB - t, tA, alfa, beta);

        if (max) {
            alfa = std::max(alfa, val);
        } else {
            beta = std::min(beta, val);
        }
        if (alfa >= beta) {
            break;
        }
    }

    if (max) {
        return alfa;
    } else {
        return beta;
    }
}

float AiEnemy::calculate(const AiEnemy::Data &data) {
    auto &user = data.user[false];
    auto &ai = data.user[true];

    auto calculate = [](decltype(data.user[0]) &u) {
        if (u.base_life == 0) { return 0.f; }

        float points = 0;
        points += u.resources[0];
        points += u.resources[1];
        points += u.resources[2];

        points += static_cast<float>(u.attack_building) * 2100.f; //2100 of resources
        points += static_cast<float>(u.resource_building) * 1800.f; //1800 of resources

        points += static_cast<float>(u.attack_ship) * 300.f; //300 of resources
        points += static_cast<float>(u.resource_ship) * 200.f; //200 of resources

        points -= (u.buildings_life / 30.f) * 1950.f; //building have 30 in life
        points -= (u.ship_life / 10.f) * 250.f; //ship have 10 in life;
/*
        for (auto &t : u.time_to_attack_ship) {
            if (t > 0) {
                points += 70;
            }
        }

        for (auto &t : u.time_to_resource_ship) {
            if (t > 0) {
                points += 50;
            }
        }
*/
        points += u.base_life * 1000;

        return points;
    };


    return calculate(ai) - calculate(user);
}

std::pair<std::array<AiEnemy::Actions, 6>, size_t> AiEnemy::actions(const AiEnemy::Data &data, bool max) {
    const std::array<Actions, 6> tab = {Attack,
                                        Wait,
                                        Build_attack_ship,
                                        Build_resource_ship,
                                        Build_attack_building,
                                        Build_resource_building};
    std::array<Actions, 6> result{};
    size_t i = 0;

    auto &d = data.user[max];

    if (!max) { //user
        if (data.user[max].user_attacks) {
            result[0] = Attack;
            return {result, 1};
        }
    }

    for (auto &e : tab) {
        switch (e) {
            case Attack: {
                if (d.attack_ship || d.resource_ship) {
                    result[i] = Attack;
                    ++i;
                }
            }
                break;
            case Wait: {
                if (d.resource_ship) {
                    result[i] = Wait;
                    ++i;
                }
            }
                break;
            case Build_attack_building: {
                if (d.resources[(int) ResourceType::food] >= 500 && d.resources[(int) ResourceType::water] >= 1000 &&
                    d.resources[(int) ResourceType::metal] >= 600) {
                    result[i] = Build_attack_building;

                    ++i;
                }
            }
                break;
            case Build_resource_building: {
                if (d.resources[(int) ResourceType::food] >= 500 && d.resources[(int) ResourceType::water] >= 1000 &&
                    d.resources[(int) ResourceType::metal] >= 300) {
                    result[i] = Build_resource_building;

                    ++i;
                }
            }
                break;
            case Build_attack_ship: {
                if (d.resources[(int) ResourceType::food] >= 100 && d.resources[(int) ResourceType::metal] >= 200) {
                    result[i] = Build_attack_ship;

                    ++i;
                }
            }
                break;
            case Build_resource_ship: {
                if (d.resources[(int) ResourceType::food] >= 100 && d.resources[(int) ResourceType::metal] >= 100) {
                    result[i] = Build_resource_ship;

                    ++i;
                }
            }
        }
    }

    return {result, i};
}

AiEnemy::Data AiEnemy::simulate(AiEnemy::Data d, AiEnemy::Actions action, bool max, float time) {
    switch (action) {
        case Attack: {
            //TODO: !!!4 Change hara as we attack, enemy can not attack and only defence, then enmy is waiting
            d.user[0].ship_life += (d.user[1].attack_ship * 7 + d.user[1].resource_ship * 3) * time;
            d.user[1].ship_life += (d.user[0].attack_ship * 7 + d.user[0].resource_ship * 3) * time;

            auto remove = [](decltype(d.user[0]) &user) {
                while (user.ship_life >= 10 && user.attack_ship != 0) {
                    user.attack_ship -= 1;
                    user.ship_life -= 10;
                }
                while (user.ship_life >= 10 && user.attack_ship != 0) {
                    user.resource_ship -= 1;
                    user.ship_life -= 10;
                }
                if (user.ship_life >= 10) {
                    user.buildings_life += user.ship_life;
                    user.ship_life = 0;
                }
                while (user.buildings_life >= 30 && user.attack_building != 0) {
                    user.attack_building -= 1;
                    user.buildings_life -= 30;
                }
                while (user.buildings_life >= 30 && user.resource_building != 0) {
                    user.resource_building -= 1;
                    user.buildings_life -= 30;
                }
                if (user.buildings_life >= 30) {
                    user.base_life -= user.buildings_life;
                    user.buildings_life = 0;

                    user.base_life = user.base_life > 0 ? user.base_life : 0;
                }
            };

            remove(d.user[0]);
            remove(d.user[1]);

            return d;
        }
            break;
        case Wait:
            break;
        case Build_attack_building: {
            d.user[max].resources[(int) ResourceType::food] -= 500;
            d.user[max].resources[(int) ResourceType::metal] -= 600;
            d.user[max].resources[(int) ResourceType::water] -= 1000;
            d.user[max].attack_building += 1;
        }
            break;
        case Build_resource_building: {
            d.user[max].resources[(int) ResourceType::food] -= 500;
            d.user[max].resources[(int) ResourceType::metal] -= 300;
            d.user[max].resources[(int) ResourceType::water] -= 1000;
            d.user[max].resource_building += 1;
        }
            break;
        case Build_attack_ship: {
            d.user[max].resources[(int) ResourceType::food] -= 100;
            d.user[max].resources[(int) ResourceType::metal] -= 200;

            for (auto &s : d.user[max].time_to_attack_ship) {
                if (s <= 0) {
                    s = 10;
                    break;
                }
            }
        }
            break;
        case Build_resource_ship: {
            d.user[max].resources[(int) ResourceType::food] -= 100;
            d.user[max].resources[(int) ResourceType::metal] -= 100;

            for (auto &s : d.user[max].time_to_resource_ship) {
                if (s <= 0) {
                    s = 10;
                    break;
                }
            }
        }
    }

    { //Wait
        auto wait = [time](decltype(d.user[0]) &user) {
            auto res = user.resource_ship * 10.f / 3.f * time; //10 speed of gathering
            user.resources[0] += res / 3.0f;
            user.resources[1] += res / 3.0f;
            user.resources[2] += res / 3.0f;

            int i = 0;
            for (auto &r : user.time_to_resource_ship) {
                if (i >= user.resource_building) { break; }
                if (r > 0) {
                    r -= time;
                    if (r <= 0) {
                        user.resource_ship += 1;
                        r = 0;
                        ++i;
                    }
                }
            }

            i = 0;
            for (auto &r : user.time_to_attack_ship) {
                if (i >= user.attack_building) { break; }
                if (r > 0) {
                    r -= time;
                    if (r <= 0) {
                        user.attack_ship += 1;
                        r = 0;
                        ++i;
                    }
                }
            }
        };

        wait(d.user[0]);
        wait(d.user[1]);
    }

    return d;
}

float AiEnemy::NextActionTime(const AiEnemy::Data &d, bool max) {
    //Calculate time to ship build as building is too big to build

    auto &u = d.user[max];

    auto res = u.resource_ship * 10.f / 4.f; //10 speed of gathering
    auto b1 = u.resources[(int) ResourceType::food] - 100; //Ship is fastest to build
    auto b2 = u.resources[(int) ResourceType::metal] - 100;

    b2 = std::min(b1, b2);

    if (b2 < 0 && res != 0) {
        return -b2 / res;
    }

    return 10; //We don't have resources ship or we have enough resources, but we wait for building
}
