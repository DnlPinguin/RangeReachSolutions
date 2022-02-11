
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/github_username/repo_name">
    <img src="https://user-images.githubusercontent.com/32873742/133936423-b4c08503-b738-4f85-89f5-3ba221ecc4dc.png" alt="Logo" >
  </a>

  <h3 align="center">Range Reach Solutions</h3>

  <p align="center">
    project_description
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/DnlPinguin/RangeReachSolutions">View Demo</a>
    ·
    <a href="https://github.com/DnlPinguin/RangeReachSolutions/issues">Report Bug</a>
    ·
    <a href="https://github.com/DnlPinguin/RangeReachSolutions/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

Here's a blank template to get started:
**To avoid retyping too much info. Do a search and replace with your text editor for the following:**
`github_username`, `repo_name`, `twitter_handle`, `email`, `project_title`, `project_description`


### Built With

* [C++]()
* [Boost]()
* [NetworkX]()



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites
1. Store Social And Spatial  Data in data/raw
  - `data/raw/{{filename}}_social`
  - `data/raw/{{filename}}_spatial`
2. Social Data is portrayed as following: 

| Column One  | Column Two |
| ------------- | ------------- |
| Node a | Node c |
| Node b | Node b |
| Node c | Node d |

where Column One -> Column Two represents one directed edge.

3. Spatial Data is portrayed as following:
  
| Column One  | Column Two | Column Three |
| ------------- | ------------- | ------------- |
| Node a | Position x | Position y|
| Node b | Position x | Position  |
| Node c | Position x | Position y |
### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/DnlPinguin/RangeReachSolutions/.git
   ```
2. Run make file
   ```sh
   make
   ```



<!-- USAGE EXAMPLES -->
## Usage

### Create Interval Scheme
`./Executables/createIntervalScheme {{filename}} {{numberOfThreads}}`
ADD Brnach already expanded 

### Create Queries
`./Executables/createQueries {{fileName}} {{numberOfQueries}}`

### Run Queries
- Spatial First `./Executables/runSpatialFirst {{fileName}} {{points||mbr}} {{strict||sequential}} `
- Social First `./Executables/runSocialFirst {{fileName}} {{points||mbr}} {{strict||sequential}}`
- Hybrid `./Executables/runHybrid {{fileName}} {{points||mbr}} {{area||degree}}`
- RangeReach `./Executables/runRangeReach {{fileName}} {{area||degree}}`

<!-- ROADMAP -->
## Roadmap

- [ ] Implement python graph generator
- [x] Executables for every query
- [ ] BFL for SpatialFirst Solution
- [ ] Implement Layer architecture for RangeReach
- [ ] Implement correction function for results


<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Your Name - [@twitter_handle](https://twitter.com/twitter_handle) - email

Project Link: [https://github.com/github_username/repo_name](https://github.com/github_username/repo_name)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* []()
* []()
* []()


