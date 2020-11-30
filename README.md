<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url] 
<!-- [![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<br />
<p align="center">


  <h3 align="center">Server Evaluator</h3>

  <p align="center">
    <a href="https://github.com/ignacimo98/server-evaluator">View Project</a>
    ·
    <a href="https://github.com/ignacimo98/server-evaluator/issues">Report Bug</a>
    ·
    <a href="https://github.com/ignacimo98/server-evaluator/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Compilation](#compilation)
* [Usage](#usage)
* [License](#license)
* [Contact](#contact)




<!-- ABOUT THE PROJECT -->
## About The Project




This is an experiment to evaluate the performance of different server models for a server-client architecture, with one server being totally sequential, another one creates a thread to process the requests, and one last one has a pre created pool of threads which get assigned a request when it arrives. 


### Built With

* C: Standard _gnu17_
* Python: Version >3.5
* matplotlib




<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

In order to compile and run the project, the following dependencies are needed.
* gcc
* make
* python and matplotlib

### Compilation

1. Clone the repo
```sh
git clone https://github.com/ignacimo98/server-evaluator.git
```
2. Change directory to the project's base directory
```sh
cd server-evaluator
```
3. Compile the project
```sh
make
```



<!-- USAGE EXAMPLES -->
## Usage

In order to run the project, make sure you're still on the project's base directory and execute it.
```sh
make test
```




<!-- LICENSE -->
## License

Distributed under the GNU GPLv3 License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

* Ignacio Mora - [@nachomora_](https://twitter.com/nachomora_) - ignacimo98@gmail.com
* Daniel Castro - [@danicast_c](https://twitter.com/danicast_c) - danielsony1311@gmail.com
* Jung
* Jorge

Project Link: [https://github.com/ignacimo98/server-evaluator](https://github.com/ignacimo98/server-evaluator)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/ignacimo98/server-evaluator.svg?style=flat-square
[contributors-url]: https://github.com/ignacimo98/server-evaluator/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/ignacimo98/server-evaluator.svg?style=flat-square
[forks-url]: https://github.com/ignacimo98/server-evaluator/network/members
[stars-shield]: https://img.shields.io/github/stars/ignacimo98/server-evaluator.svg?style=flat-square
[stars-url]: https://github.com/ignacimo98/server-evaluator/stargazers
[issues-shield]: https://img.shields.io/github/issues/ignacimo98/server-evaluator.svg?style=flat-square
[issues-url]: https://github.com/ignacimo98/server-evaluator/issues
[license-shield]: https://img.shields.io/github/license/ignacimo98/server-evaluator.svg?style=flat-square
[license-url]: https://github.com/ignacimo98/server-evaluator/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/github_username
[product-screenshot]: images/screenshot.png