var host = window.location.hostname;
var ws = new WebSocket("ws://"+host+":9002");

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

var resource_names = [];
var city_names = [];
var building_names = [];
var army_names = [];

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
                if(city != null && resource != null)
                    el.innerHTML = data["cities"][city]["resources"][resource]["price"];
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
