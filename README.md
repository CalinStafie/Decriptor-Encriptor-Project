# Decriptor-Encriptor Project
#### Frontend

##### HTML si CSS 

- Fisiere separate pentru HTML si CSS: "index.html", "style.css"
- In interiorul documentelor HTML, sa se foloseasca minim 4 taguri semantice
```c

```
- Stilurile CSS sa fie definite folosind clase direct pe elementele care trebuie stilizate (minim 80% din selectori) 
- Layout-ul sa fie impartit in minim 2 coloane si sa fie realizat cu Flexbox si/sau CSS grid

```css   
header{
    display:grid;
    grid-template-columns : 30% 70%;
    justify-items:center;
    align-items:center;
    border-bottom:  #dfe2e2 4px solid;
}   
 .main-section{
    color:#0f0f3d;
    height: 100%;
    display:grid;
    grid-template-columns: 150px 1fr 150px;
    grid-template-areas:". main-div .";
}
.film{
    margin:10px;
    display:grid;
    grid-template-rows: fit-content(35%)  1fr 40px;
    height:500px;
    background-color:rgb(241, 250, 238, 0.8);
    border-radius:12px;
}

```
- Site-ul sa fie responsive, respectand rezolutiile urmatoarelor dispozitive folosind media queries: 
  - telefon mobil - latime mai mica 768px
  - tableta - latime intre 768px si 1280px
  - desktop - latime mai mare de 1280px
 ```css
@media only screen and (min-width: 769px) and (max-width: 1280px) {
    /* For tablets: */
    .main-section{
        color:#0f0f3d;
        height: 100%;
        display:grid;
        grid-template-columns: 80px 1fr 80px;
        grid-template-areas:". main-div .";
    }
    .main-div-search{
        grid-area:main-div;
        text-align:center;
        margin-top: 50px;
        margin-bottom:50px;
        background-color: rgb(241, 250, 238, 0.6);   
        display:grid;
        grid-template-columns: 1fr 1fr;
    }
    .film{
        margin:5px;
        display:grid;
        grid-template-rows: fit-content(35%)  1fr 60px;
        height:500px;
        background-color:rgb(241, 250, 238, 0.8);
    }

}

@media only screen and (max-width: 768px) {
    /* For mobile phones: */
    .add-poza-titlu{
        display:grid;
        grid-template-columns: 1fr;
        grid-template-rows:1fr 1fr;
    }
    .film{
        margin:5px;
        display:grid;
        grid-template-rows: fit-content(35%)  1fr 60px;
        height:600px;
        background-color:rgb(241, 250, 238, 0.8);
    }
    .main-section{
        color:#0f0f3d;
        height: 100%;
        display:grid;
        grid-template-columns: 50px 1fr 50px;
        grid-template-areas:". main-div .";
    }
    .main-div-search{
        grid-area:main-div;
        text-align:center;
        margin-top: 50px;
        margin-bottom:50px;
        background-color: rgb(241, 250, 238, 0.6);   
        display:grid;
        grid-template-columns: 1fr;
    }
}
```
##### Javascript 

- Fisier separat JavaScript: "script.js"
- Manipularea DOM-ului (crearea, editarea si stergerea elementelor/nodurilor HTML)
```js
//creare
let div_id = document.createElement("div");
div_id.id = films[i].id;
div_id.className = 'film';

//stergere
mainSection.removeChild(mainSection.firstChild);

//editare
let div_descriere = document.createElement("div");
div_descriere.className = "descriere";
div_descriere.innerText = "Actors: " + films[i].author + "\n Description: " + films[i].description;
```
- Folosirea evenimentelor JavaScript declansate de mouse/tastatura 
```js
document.getElementById("home-btn").addEventListener("click", goHome);
document.getElementById("search-films-btn").addEventListener("click", goSearchfilms);
document.getElementById("add-films-btn").addEventListener("click", goAddfilms);
```
- Utilizarea AJAX: GET, POST, PUT, DELETE
```js
// GET
fetch('http://localhost:3000/films')
    .then(function (response) {
        
        response.json().then(function (films) {
            for(let i = 0; i < films.length; i++)
            {
                let div_id = document.createElement("div");
                div_id.id = films[i].id;
                div_id.className = 'film';

                let div_poz_titlu = document.createElement("div");
                div_poz_titlu.className = 'poza-titlu';
                
                let div_desc = document.createElement("div");
                div_desc.className = 'desc-produs';
                
                let img = document.createElement("img");
                img.className = 'img';
                img.alt = 'film'
                img.src = films[i].img;
                
                div_desc.appendChild(img);
                
                let div_titlu_pret = document.createElement("div");
                div_titlu_pret.className = "titlu-pret";

                let h3 = document.createElement("p");
                h3.className = 'titlu';
                h3.innerText = films[i].name;

                let h4 = document.createElement("p");
                h4.className = 'pret';
                h4.innerHTML = '<i class="fa fa-star" aria-hidden="true"></i>' + ' ' + films[i].price + ' / 10';

                div_titlu_pret.appendChild(h3);
                div_titlu_pret.appendChild(h4);
                div_poz_titlu.appendChild(div_desc);
                div_poz_titlu.appendChild(div_titlu_pret);

                let div_descriere = document.createElement("div");
                div_descriere.className = "descriere";
                div_descriere.innerText = "Actors: " + films[i].author + "\n Description: " + films[i].description;

                let btn = document.createElement("button");
                btn.className = "btn-buy";
                btn.type = "button";
                btn.innerText = "Details";

                div_id.appendChild(div_poz_titlu);
                div_id.appendChild(div_descriere);
                div_id.appendChild(btn);
                
                btn.addEventListener('click', function () {
                    generateSearchFilms(films[i]); // de pus functia de generare
                });

                main_div.appendChild(div_id);

            }
            
        });
    });
    
// POST
function postfilm(postObject)
{

    fetch('http://localhost:3000/films', {
        method: 'post',
        headers: {
            "Content-type": "application/json"
        },
        body: JSON.stringify(postObject)
    }).then(function () {
        localStorage.setItem("onSearch", "false");
        goSearchfilms();
    });

}

// DELETE
document.getElementById("delete-film-btn").addEventListener('click',function()
    {
        fetch(`http://localhost:3000/films/${film.id}`, {
        method: 'DELETE',
    }).then(function () {

        goModifiy();
    }); 
    });
    
 // PUT
fetch(`http://localhost:3000/films/${film.id}`, {
            method: 'PUT',
            headers: {
                "Content-type": "application/json"
            },
            body: JSON.stringify(Obj)
        }).then(function () {
        goModifiy();
    });

```
- Folosirea localStorage 
```js
localStorage.setItem("onSearch", "false");
localStorage.setItem("loged","false");
localStorage.setItem("nume", txt);
localStorage['loged'] == 'true'
```
 #### Backend API 
- Creare server Backend 
```js
const express = require("express");
```
- CRUD API (Create, Read, Update si Delete) pentru a servi Frontend-ului 
```js
// CREATE
app.post("/films", (req, res) => {
  const filmsList = readJSONFile();
  const newFilm = req.body;
  newFilm.id = uuidv1();
  const newFilmList = [...filmsList, newFilm];
  writeJSONFile(newFilmList);
  res.json(newFilm);
});

// READ ONE
app.get("/films/:id", (req, res) => {
  const filmsList = readJSONFile();
  const id = req.params.id;
  let idFound = false;
  let foundFilm;

  filmsList.forEach(film => {
    if (id == film.id) {
      idFound = true;
      foundFilm = film
    }
  });

  if (idFound) {
    res.json(foundFilm);
  } else {
    res.status(404).send(`Film ${id} was not found`);
  }
});

// READ ALL
app.get("/films", (req, res) => {
  const filmsList = readJSONFile();
  res.json(filmsList);
});

// UPDATE
app.put("/films/:id", (req, res) => {
  let filmsList = readJSONFile();
  let id = req.params.id;
  let newFilm = req.body;
  newFilm.id = id;
  idFound = false;

  const newFilmsList = filmsList.map((film) => {
     if (film.id == id) {
       idFound = true;
       return newFilm
     }
    return film
  })
  
  writeJSONFile(newFilmsList);

  if (idFound) {
    res.json(newFilm);
  } else {
    res.status(404).send(`Film ${id} was not found`);
  }

});

 // DELETE
app.delete("/films/:id", (req, res) => {
    const filmsList = readJSONFile();
    const id = req.params.id;
    const newFilmsList = filmsList.filter((film) => film.id != id)
  
    if (filmsList.length !== newFilmsList.length) {
      res.status(200).send(`Film ${id} was removed`);
      writeJSONFile(newFilmsList);
    } else {
      res.status(404).send(`Film ${id} was not found`);
    }
  });
  
```
