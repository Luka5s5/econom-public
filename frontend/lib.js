var host = window.location.hostname;
var ws = new WebSocket("ws://"+host+":9002");

console.log("lib loaded");

function city_update(x){
    let ll = document.getElementsByClassName("city-dependent");
    for(let el of ll){
        el.setAttribute("data-city", x);
    }
}

function team_update(x){
    let ll = document.getElementsByClassName("team-dependent");
    for(let el of ll){
        el.setAttribute("data-team", x);
    }
}

ws.onopen = function() {
    console.log("onopen... ");
    ws.send(JSON.stringify({"type": "update-all"}));
};

//id

//classes
//team-selector
//resource-limit
//city-selector
//resource-price
//city-dependent
//team-dependent
//treaty
//strategy
//building-lvl
//army-numbers
//resource-numbers
//victory-points

//attacker
//defender
//atk-list
//def-list
//atk-death
//def-death
//atk-army
//def-army
//army-balance

var resource_names = [];
var city_names = [];
var building_names = [];
var army_names = [];
var team_names = [];

ws.onmessage = function (evt) { 
    console.log("onmessage: " + evt.data);
    var data = JSON.parse(evt.data);
    if(data["type"] != null && data["type"] == "response"){
        alert(data["message"]);
        return;
    }
    if(data["resource_names"] != null){
        resource_names = data["resource_names"];
    }
    if(data["building_names"] != null){
        building_names = data["building_names"];
    }
    if(data["army_names"] != null){
        army_names = data["army_names"];
    }
    if(data["cities"] != null){
        for(let city of data["cities"]){
            city_names[Number(city["id"])] = city["name"];
        }
    }
    if(data["teamlist"] != null){
        for(var team of data["teamlist"]){
            team_names[team["id"]] = team["name"];
        }
        var l = document.getElementsByClassName("team-selector");
        if(l != null){
            for(var selector of l){
                if(selector.getElementsByTagName("option") != null && selector.getElementsByTagName("option").length != 0)
                    continue;
                for(var team of data["teamlist"]){
                    var el = document.createElement("option");
                    el.textContent = team["name"];
                    el.value = Number(team["id"]);
                    selector.appendChild(el);
                }
            }
        }
    }
    update();
    if(data["teamlist"] != null){
        var l = document.getElementsByClassName("team-selector");
        if(l.length != 0)
            team_update(l[0].value);
        var l = document.getElementsByClassName("strategy");
        if(l != null){
            for(var btn of l){
                let id = btn.dataset.team;
                let strattype = btn.dataset.strat;
                if(id != null){
                    if(data.teamlist[id].strategy == strattype){
                        btn.classList.remove(btn.dataset.hasnt);
                        btn.classList.add(btn.dataset.has);
                    }else{
                        btn.classList.remove(btn.dataset.has);
                        btn.classList.add(btn.dataset.hasnt);
                    }
                }
            }
        }
        var l = document.getElementsByClassName("building-lvl");
        if(l != null){
            for(let lab of l){
                let id = lab.dataset.team;
                let type = lab.dataset.type;
                if(id != null){
                    lab.dataset.lvl = data.teamlist[id].buildings[type];
                    lab.innerHTML = data.teamlist[id].buildings[type];
                }
            }
        }
        var l = document.getElementsByClassName("army-numbers");
        if(l != null){
            for(let lab of l){
                let id = lab.dataset.team;
                let type = lab.dataset.type;
                if(id != null){
                    lab.innerHTML = data.teamlist[id].army[type];
                }
            }
        }
        var l = document.getElementsByClassName("resource-numbers");
        if(l != null){
            for(let lab of l){
                let id = lab.dataset.team;
                let type = lab.dataset.type;
                if(id != null){
                    lab.innerHTML = data.teamlist[id].resources[type];
                }
            }
        }
        var l = document.getElementsByClassName("money-numbers");
        if(l != null){
            for(let lab of l){
                let id = lab.dataset.team;
                let type = lab.dataset.type;
                if(id != null){
                    lab.innerHTML = data.teamlist[id].money[type].toFixed(2);
                }
            }
        }
        var l = document.getElementsByClassName("treaty");
        if(l != null){
            for(var btn of l){
                let id1 = btn.dataset.team1;
                let id2 = btn.dataset.team2;
                let treatytype = btn.dataset.treatytype;
                let check = false;
                if(id1 != null && id2 != null){
                    for(let a of data.teamlist[id1].treaties[treatytype]){
                        if(a == id2){
                            check = true;
                            break;
                        }
                    }
                }
                if(check){
                    btn.classList.remove(btn.dataset.hasnt);
                    btn.classList.add(btn.dataset.has);
                }else{
                    btn.classList.remove(btn.dataset.has);
                    btn.classList.add(btn.dataset.hasnt);
                }
            }
        }
        var l = document.getElementsByClassName("victory-points");
        if(l != null){
            for(let lab of l){
                let id = lab.dataset.team;
                if(id != null){
                    lab.innerHTML = data.teamlist[id].victory_points;
                }
            }
        }
    }
    if(data["cities"] != null){
        var l = document.getElementsByClassName("city-selector");
        if(l != null){
            for(var selector of l){
                if(selector.getElementsByTagName("option") != null && selector.getElementsByTagName("option").length != 0)
                    continue;
                for(var city of data["cities"]){
                    var el = document.createElement("option");
                    el.textContent = city["name"];
                    el.value = Number(city["id"]);
                    selector.appendChild(el);
                }
            }
            if(l.length != 0)
                city_update(l[0].value);
        }
        var ll = document.getElementsByClassName("resource-limit");
        if(ll != null){
            for(var el of ll){
                var city = el.getAttribute("data-city");
                var resource = el.getAttribute("data-resource");
                if(city != null && resource != null)
                    el.innerHTML = data["cities"][city]["resources"][resource]["limit"];
            }
        }
        var ll = document.getElementsByClassName("resource-price");
        if(ll != null){
            for(var el of ll){
                var city = el.getAttribute("data-city");
                var resource = el.getAttribute("data-resource");
                if(city != null && resource != null){
                    el.innerHTML = data["cities"][city]["resources"][resource]["price"].toFixed(2);
                }
            }
        }
        if(data.wars != null && data.wars.length > 0){
            let war = data.wars[0];
            let sum_atk = 0;
            for(let a of war.attacker_army){
                sum_atk += a;
            }
            let sum_def = 0;
            for(let a of war.defender_army){
                sum_def += a;
            }
            var ll = document.getElementsByClassName("attacker");
            if(ll != null){
                for(let el of ll){
                    el.innerHTML = team_names[war.attacker];
                }
            }
            var ll = document.getElementsByClassName("atk-list");
            if(ll != null){
                for(let el of ll){
                    while (el.firstChild) {
                        el.removeChild(el.lastChild);
                    }
                    for(let id of war.attackers_list){
                        let li = document.createElement("li");
                        li.innerHTML = team_names[id];
                        el.appendChild(li);
                    }
                }
            }
            var ll = document.getElementsByClassName("atk-death");
            if(ll != null){
                for(let el of ll){
                    el.style.width = (100*sum_atk/war.attackers_initial_army)+"%";
                }
            }
            var ll = document.getElementsByClassName("atk-army");
            if(ll != null){
                for(let el of ll){
                    let type = el.dataset.type;
                    el.style.width = (100*war.attacker_army[type]/sum_atk)+"%";
                }
            }
            var ll = document.getElementsByClassName("defender");
            if(ll != null){
                for(let el of ll){
                    el.innerHTML = team_names[war.defender];
                }
            }
            var ll = document.getElementsByClassName("def-list");
            if(ll != null){
                for(let el of ll){
                    while (el.firstChild) {
                        el.removeChild(el.lastChild);
                    }
                    for(let id of war.defenders_list){
                        let li = document.createElement("li");
                        li.innerHTML = team_names[id];
                        el.appendChild(li);
                    }
                }
            }
            var ll = document.getElementsByClassName("def-death");
            if(ll != null){
                for(let el of ll){
                    el.style.width = (100*sum_def/war.defenders_initial_army)+"%";
                }
            }
            var ll = document.getElementsByClassName("def-army");
            if(ll != null){
                for(let el of ll){
                    let type = el.dataset.type;
                    el.style.width = (100*war.defender_army[type]/sum_def)+"%";
                }
            }
            var ll = document.getElementsByClassName("army-balance");
            if(ll != null){
                for(let el of ll){
                    el.style.width = (100*sum_atk/(sum_def+sum_atk))+"%";
                }
            }
        }
    }
};
                
ws.onclose = function() { 
    alert("Connection is closed..."); 
};

var l = document.getElementsByClassName("city-selector");
for(var selector of l){
    selector.onchange = function(){
        city_update(this.value);
        ws.send(JSON.stringify({"type": "update-all"}));
    }
}

function getCookie(cname) {
  let name = cname + "=";
  let ca = document.cookie.split(';');
  for(let i = 0; i < ca.length; i++) {
    let c = ca[i];
    while (c.charAt(0) == ' ') {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(name.length, c.length);
    }
  }
  return "";
}

function wsSend(msg){
    msg.token = getCookie("token");
    ws.send(JSON.stringify(msg));
}


