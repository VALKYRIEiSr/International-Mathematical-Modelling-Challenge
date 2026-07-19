# Global Sports League: A Fair Scheduling Model for International Sports Leagues
**IM²C 2025 | Team #2025061 | 🌍 International Honorable Mention | 🥇 National 1st Place**

## Project Overview
This project presents a mathematical model for constructing a fair, practical, and scalable schedule for a Global Sports League (GSL) — developed for the 2025 International Mathematical Modeling Challenge (IM²C). The model addresses equitable competition scheduling across 20 and 24 team leagues, accounting for travel distance, recovery intervals, matchup fairness, and environmental/economic sustainability.

The system combines a modified ELO rating framework, proportional continental team representation (via the Hare-Niemeyer method), spherical trigonometry for optimal match-location selection, and a graph-theory-based scheduling algorithm (K-regular graph construction with a modified DSATUR vertex-coloring heuristic) to generate a complete, balanced league calendar. The model was further stress-tested by expanding the league from 20 to 24 teams and generalized for applicability across other team-based sports.

## Technical Research
The full mathematical derivations, model construction, results, and appendices are documented in the competition report:
* **Competition Report:** [`IMMC Report.pdf`](./IMMC%20Report.pdf)

## Technical Specifications
* **Team Selection:** Proportional continental representation via the Hare-Niemeyer (largest remainder) method, combined with an ELO-based shortlisting system
* **Rating System:** A modified, piecewise-symmetric ELO function derived from a logistic CDF approximation, accounting for goal-difference magnitude and cross-continental match weighting — designed and developed by Hariz Zoran Farooq
* **Match Location:** Spherical trigonometry (haversine formula) used to compute the geographic midpoint between two teams and select the nearest registered stadium
* **Sustainability:** A weighted "Green Score" system incentivizing travel efficiency, sustainable kit sourcing, operational practices, and innovation, capped at 15 ELO points per team
* **Scheduling:** League phase modeled as a K-regular graph; minimum match-weeks bounded via Brooks' Theorem; matchups generated using a modified DSATUR graph-coloring algorithm (prioritizing lower-saturation vertices for improved schedule balance)
* **Validation:** Includes a documented comparison between an earlier, less balanced DSATUR implementation and the improved version used in the final model

## Repository Structure
* `Code`: C++ implementation of the model
  * `Haversine.cpp`: Computes match-location midpoints and nearest registered stadium via the haversine formula
  * `Implement.cpp`: ELO rating system implementation, including score updates from match data
  * `Schedule.cpp`: Modified DSATUR graph-coloring algorithm for generating the league match schedule
* `Matches`: Historical match data used to compute ELO ratings
* `IMMC Report.pdf`: Full competition report — literature review, model derivation, scheduling results, model expansion/generalization, and appendices

## Usage Summary
1. **ELO Calculation:** Run `Implement.cpp`, pointing it at the `Matches` directory to compute team ratings from historical match results.
2. **Match Location:** Run `Haversine.cpp` with team/stadium coordinates to determine match venues.
3. **Scheduling:** Run `Schedule.cpp` to generate the full league schedule using the modified DSATUR algorithm across the tiered team structure.

## Licensing
No formal license has been applied to this repository yet. All rights reserved to the developers unless otherwise specified.

---
**Team:** #2025061
**Code Developer:** Hariz Zoran Farooq (ELO rating system design and implementation)
**Teammates:** Murtaza Younus, Zoraiz Khalid, Aleena Khokkar
**Contact:** harizzoranfarooq@gmail.com
**Competition:** IM²C (International Mathematical Modeling Challenge) 2025 — International Honorable Mention, National 1st Place
