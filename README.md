# Multi-Agent-Simulation-with-Spread-of-Infection
## Summary
Using a multi-agent simulation to simulate the infection spread model. In case of contact, the agent ID of the contacted agent was recorded so that the source of infection could be identified.

## Implementation Details
* The agent is configured to transition its "physical condition" for each My Loop as it does in daily life. (A transition diagram of the physical condition is shown below. More rigorous consideration should be given to parameter settings.)

<img width="500" alt="infection_rate" src="/images/infection_rate_Eng.png">

* There is a 10% chance of becoming ill and a 30% chance of contracting the disease if in contact with a sick agent.

※ This time, instead of assigning a fixed number of infected agents in advance, agents in poor physical condition are randomly exposed to the disease. (Spontaneous disease onset was introduced to account for cases where the source of infection cannot be identified.)
* The IDs of contacted agents are recorded to enable contact management.
* The concept of "self-restraint" was added, and the user was set to reduce the speed of movement by half when he/she is poor health, and to stop when infected.


## Multi Agent Simulation DEMO
<img width="800" alt="MaltiAgentSimulation" src="/images/MaltiAgentSimulation.gif">

Relationship between physical condition and color
- ![#00ff00](https://via.placeholder.com/15/00ff00/000000?text=+) `Green : good`
- ![#ffffff](https://via.placeholder.com/15/ffffff/000000?text=+) `White : normal`
- ![#0000ff](https://via.placeholder.com/15/0000ff/000000?text=+) `Blue: poor health`
- ![#ff00ff](https://via.placeholder.com/15/ff00ff/000000?text=+) `Purple: infected`
- ![#ff0000](https://via.placeholder.com/15/ff0000/000000?text=+) `(Red: at contact)`


## Comparison of the number of cases of infection with and without self-restraint
**Although the spread of infection was slowed, it did not decrease.**

<img width="800" alt="infection_transition" src="/images/infection_transition.png">

* Wave of infections comes late in "with restraint"
  * Possible Factors : Reduction in frequency of contact is achieved by stopping agents in the infected state
* The number of infected persons itself has not been suppressed
  * Possible Factors : The number of infected persons has been mostly accounted for by the number of randomly generated infected persons.
## A function to select an arbitrary agent and search the agent's contact history is implemented in GAS.
<img width="800" alt="MaltiAgentSimulation_spreadsheet" src="/images/MaltiAgentSimulation_spreadsheet.gif">
By specifying an ID when executing the script, you can obtain a list of IDs of agents contacted by the agent with that ID.

---

# Improvement items for more realistic simulation requirements
* The probability of determining physical condition is constant.
  * Proposed Improvement
    * Calculate based on biorhythm[1] compatibility calculation.
    * Vary the probability depending on the day's behavior.

* Evaluate the number of infected people in more environments.
  * Vary the probability of infection and consider the results
  * Introduce the concept of "antibodies
  * Experiment what happens when the action of going to the hospital is incorporated.
    * Whether the number of infected people temporarily increases as more people gather at the hospital etc...

[1] Hypothesis that physiological states, emotions, intelligence, etc. of living organisms change according to periodic patterns.
