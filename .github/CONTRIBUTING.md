# Contributing to ```CMS_GEM_Analysis_Framework```
The following are a set of guidelines to help you in contributing to the ```CMS_GEM_Analysis_Framework``` code base.

## How to start developing
Start by forking the central [```CMS_GEM_Analysis_Framework```](https://github.com/bdorney/CMS_GEM_Analysis_Framework) repository.
Once you have your fork, then you can check it out to wherever you will be developing.

### Workflow
We have been utilizing a very helpful guideline for our development model outlined here: [```git-flow```](http://nvie.com/posts/a-successful-git-branching-model/)
The basic idea is the following:
* fork from [bdorney/CMS_GEM_Analysis_Framework](https://github.com/bdorney/CMS_GEM_Analysis_Framework)
* create a branch to develop your particular feature (based off of ```develop```, or in some cases, the current ```release``` branch)
  * ```hotfix``` may be created from ```master``` or the most recent release version ```release_vX.Y```
  * once that feature is completed, create a pull request
* ```master``` should *always* be stable
  * Do *not* commit directly onto ```master``` or ```develop```, and ensure that your ```master``` and ```develop``` are up-to-date with ```bdorney``` before starting new developments

* Some generally good guidelines(though this post recommends *not* using the ```git-flow``` model[](https://juliansimioni.com/blog/three-git-commands-you-should-never-use/)
  * *Never* use ```git commit -a```
  * *Avoid* ```git commit -m``` over ```git commit -p``` or ```git commit```, as it will force you to think about your commit message
    * Speaking of... commit messages should be descriptive, not like a novel, but concise and complete.  If they reference an issue or PR, please include that information.
  * *Prefer* ```git rebase``` over ```git pull```

### Coding Style
* Avoid using ```tab```s, use an editor that is smart enough to convert all ```tab```s to ```space```s
* Current convention is 4 ```space```s per ```tab``` for ```python``` and ```c++``` code
* Unnecessary white space between lines is to be avoided, a maximum of a single empty line between code blocks
* Trailing white space after code written on a line should be avoided
* In general commented code should not be committed to the repository (reduction in current repo needed)

#### Naming Conventions For C++ STL Objects
For those classes/data types/containers present in the STL the following naming conventions
should be used:

TYPE | CONVENTION
---- | ----------
`char` | The character `c` should start the object name, e.g. `cName`.
`int` | The character `i` should start the object name, e.g. `iNumBins`
`double` | The character `d` should start the object name, e.g. `dPeakPos`
`float` | The character `f` should start the object name, e.g. `fPeakPos`
`map` | For a `std::map<T1 key, T2 val>` the sequence `map_` should start the object name followed by the character/sequence for type T2 if a convention already exists for it.  Right now T1 is mostly int type and is not specified.  If no convention for T2 exists use a starting sequence that is easily identifiable. e.g. `map_strName` or `map_hClustHisto`
`multiset` | For a `std::multiset<T>` the sequence `mset_` should start the object name followed by the character/sequence for type T if a convention already exists for it. if no convention for T exists use a starting sequnce that is easily identified as described in the `std::map` description above.
`shared_ptr` | For `std::shared_ptr<T>` follow the convention for type `T`.
`short` | The sequence `s` should start the object name, e.g. `sADC`
`string` | The sequence `str` should start the object name, e.g. `strName`
`vector` | Follow the convention for `std::map` but the starting sequence `vec_` should be used instead. e.g. `vec_strName` or `vec_hClustHistos`

#### Naming Convention for ROOT Objects
For those classes in `ROOT` that are the following listed classes, or inherit from the following listed classes, use the naming conventions outlined below:

TYPE | CONVENTION
---- | ----------
`TH1` | The character `h` should start the object name, e.g. `hClustADC`
`TH2` | As `TH1` but the physical observables of the y & x axes should be included in the object name in "y vs. x" format, e.g. `hEta_ClustADC_v_ClustPos`
`TF1` | The sequence `fit` should start the object name, e.g. `fitSlice_ClustADC`
`TGraph` | The character `g` should start the object name, e.g. `gClustADC`
`TGraph2D` | The character `g2D` should start the object name, e.g. `g2D_RespFitPkPos`

### Testing
Tests should follow this general structure, for changes to:
* Analysis code you should analyze TTree files before and after your changes to again show results meet expectations
* Comparison, plotting, and visualization code you should show the current functionality is maintained and still produces the correct outputs
* Reconstruction code you should reco raw data and then analyze it before and after your changes to show that the results meet expectations

In all cases you should attach plots to your pull requests showing before and after to either show 1) you fixed a bug, or 2) correctly implemented a new feature while maintaining the current code.

## Making a pull request
Once you have tested your code, you are ready to make a pull request.  If it references an issue or another pull request, make sure to include that information.

Additionally the description of your pull request should describe what was done, how it affects the user, and if new functionality is added examples should be shown.

In all cases you should be updating the README.txt and the CHANGELOG.txt included in the repository.

### Using Labels
#### Issue and Pull Request Labels
There are several labels used to track issues.  Unfortunately, due to the way that github is set up, general users are not
able to add these labels.  As such, they are outlined here, and when creating an issue or pull request, should be referenced
in the title so that the maintainers can apply the appropriate label easily.

| Label name | `bdorney/CMS_GEM_Analysis_Framework` :mag_right: | `bdorney` :mag_right: | Description |
| ---------- |:------------------------------------------ |:--------------------------------- |:----------- |
| `Type: Bug` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-bug] for issues | search [`bdorney`][search-bdorney-label-bug] for issues | Issue reports a `bug`, and supplementary information, i.e., how to reproduce, useful debugging info, etc. |
| `Type: Bugfix` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-bugfix] for issues | search [`bdorney`][search-bdorney-label-bugfix] for issues | Issue reports a `bugfix`, and references the bug issue |
| `Type: Duplicate` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-duplicate] for issues | search [`bdorney`][search-bdorney-label-duplicate] for issues | Issue will be tagged as `duplicate`, and a reference to the initial issue will be added|
| `Type: Enhancement` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-enhancement] for issues | search [`bdorney`][search-bdorney-label-enhancement] for issues | Issue reports an `enhancement` |
| `Type: Feature Request` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-feature-request] for issues | search [`bdorney`][search-bdorney-label-feature-request] for issues | Issue contains a `feature-request` |
| `Type: Maintenance` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-maintenance] for issues | search [`bdorney`][search-bdorney-label-maintenance] for issues | Issue reports a `maintenance` or `maintenance` request |
| `Type: New Tag` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-new-tag] for issues | search [`bdorney`][search-bdorney-label-new-tag] for issues | Issue reports a bug, and supplementary information, i.e., how to reproduce, useful debugging info, etc. |
| `Type: Question` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-question] for issues | search [`bdorney`][search-bdorney-label-question] for issues | Issue raises a question, though it will generally be better to contact on mattermost |
| `Type: Answer` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-label-answer] for issues | search [`bdorney`][search-bdorney-label-answer] for issues | Issue will answer a previously referenced question|

#### Issue and Pull Request Labels
Maintainers will (hopefully) attach a priority based on the information given in the issue/PR.

| Label name | `bdorney/CMS_GEM_Analysis_Framework` :mag_right: | `bdorney` :mag_right: | Description |
| ---------- |:------------------------------------------ |:--------------------------------- |:----------- |
| `Priority: Low` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-priority-low] for issues | search [`bdorney`][search-bdorney-priority-low] for issues | Priority `low` assigned to issue/PR |
| `Priority: Medium` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-priority-medium] for issues | search [`bdorney`][search-bdorney-priority-medium] for issues | Priority `medium` assigned to issue/PR |
| `Priority: High` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-priority-high] for issues | search [`bdorney`][search-bdorney-priority-high] for issues | Priority `high` assigned to issue/PR |
| `Priority: Critical` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-priority-critical] for issues | search [`bdorney`][search-bdorney-priority-critical] for issues | Priority `critical` assigned to issue/PR |

#### Pull Request Status Labels
Maintainers will (hopefully) properly migrate issues and pull requests through the various stages on their path to resolution.

| Label name | `bdorney/CMS_GEM_Analysis_Framework` :mag_right: | `bdorney` :mag_right: | Description |
| ---------- |:------------------------------------------ |:--------------------------------- |:----------- |
| `Status: Blocked` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-blocked] for issues | search [`bdorney`][search-bdorney-status-blocked] for issues | Issue/PR `blocked`: depends on some other issue/PR (should be referenced) |
| `Status: Pending` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-pending] for issues | search [`bdorney`][search-bdorney-status-pending] for issues | Issue/PR `pending`: acknowledged, ready to be reviewed |
| `Status: Accepted` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-accepted] for issues | search [`bdorney`][search-bdorney-status-accepted] for issues | Issue/PR `accepted`: accepted |
| `Status: Completed` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-completed] for issues | search [`bdorney`][search-bdorney-status-completed] for issues | Issue/PR `completed`: ready for inclusion |
| `Status: Invalid` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-invalid] for issues | search [`bdorney`][search-bdorney-status-invalid] for issues | Issue/PR `invalid`: invalid, possibly can't reproduce |
| `Status: Wontfix` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-wontfix] for issues | search [`bdorney`][search-bdorney-status-wontfix] for issues | Issue/PR `wontfix`: won't be included as-is |
| `Status: Wrong Repo` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-wrong-repo] for issues | search [`bdorney`][search-bdorney-status-wrong-repo] for issues | Issue/PR `wrong-repo`: issue reported in incorrect repository |
| `Status: Help Wanted` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-help-wanted] for issues | search [`bdorney`][search-bdorney-status-help-wanted] for issues | Issue/PR `help-wanted`: call for someone to take on the task |
| `Status: Revision Needed` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-revision-needed] for issues | search [`bdorney`][search-bdorney-status-revision-needed] for issues | Issue/PR `revision-needed`: something needs to be changed before proceeding |
| `Status: On Hold` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-on-hold] for issues | search [`bdorney`][search-bdorney-status-on-hold] for issues | Issue/PR `on-hold`:  being worked on, but either stale, or waiting for inputs |
| `Status: In Progress` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-in-progress] for issues | search [`bdorney`][search-bdorney-status-in-progress] for issues | Issue/PR `in-progress`:  actively being worked on |
| `Status: Review Needed` | search [`CMS_GEM_Analysis_Framework`][search-CMS_GEM_Analysis_Framework-repo-status-review-needed] for issues | search [`bdorney`][search-bdorney-status-review-needed] for issues | Issue/PR `review-needed`: ready for inclusion, needs review |

###### Acknowledgements
* Much style and syntax of this was borrowed heavily from the [atom](https://github.com/atom/atom/blob/master/CONTRIBUTING.md) repository

[search-CMS_GEM_Analysis_Framework-repo-label-bug]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Bug%22
[search-bdorney-label-bug]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Bug%22
[search-CMS_GEM_Analysis_Framework-repo-label-bugfix]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Bugfix%22
[search-bdorney-label-bugfix]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Bugfix%22
[search-CMS_GEM_Analysis_Framework-repo-label-duplicate]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Duplicate%22
[search-bdorney-label-duplicate]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Duplicate%22
[search-CMS_GEM_Analysis_Framework-repo-label-enhancement]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Enhancement%22
[search-bdorney-label-enhancement]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Enhancement%22
[search-CMS_GEM_Analysis_Framework-repo-label-feature-request]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Feature+Request%22
[search-bdorney-label-feature-request]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Feature+Request%22
[search-CMS_GEM_Analysis_Framework-repo-label-maintenance]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Maintenance%22
[search-bdorney-label-maintenance]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Maintenance%22
[search-CMS_GEM_Analysis_Framework-repo-label-question]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Question%22
[search-bdorney-label-question]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Question%22
[search-CMS_GEM_Analysis_Framework-repo-label-answer]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+Answer%22
[search-bdorney-label-answer]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+Answer%22
[search-CMS_GEM_Analysis_Framework-repo-label-new-tag]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Type%3A+New+Tag%22
[search-bdorney-label-new-tag]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Type%3A+New+Tag%22

[search-CMS_GEM_Analysis_Framework-repo-priority-low]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Priority%3A+Low%22
[search-bdorney-priority-low]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Priority%3A+Low%22
[search-CMS_GEM_Analysis_Framework-repo-priority-medium]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Priority%3A+Medium%22
[search-bdorney-priority-medium]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Priority%3A+Medium%22
[search-CMS_GEM_Analysis_Framework-repo-priority-high]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Priority%3A+High%22
[search-bdorney-priority-high]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Priority%3A+High%22
[search-CMS_GEM_Analysis_Framework-repo-priority-critical]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Priority%3A+Critical%22
[search-bdorney-priority-critical]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Priority%3A+Critical%22

[search-CMS_GEM_Analysis_Framework-repo-status-invalid]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Invalid%22
[search-bdorney-status-invalid]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Invalid%22
[search-CMS_GEM_Analysis_Framework-repo-status-wontfix]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Wontfix%22
[search-bdorney-status-wontfix]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Wontfix%22
[search-CMS_GEM_Analysis_Framework-repo-status-accepted]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Accepted%22
[search-bdorney-status-accepted]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Accepted%22
[search-CMS_GEM_Analysis_Framework-repo-status-completed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Completed%22
[search-bdorney-status-completed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Completed%22
[search-CMS_GEM_Analysis_Framework-repo-status-pending]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Pending%22
[search-bdorney-status-pending]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Pending%22
[search-CMS_GEM_Analysis_Framework-repo-status-blocked]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Blocked%22
[search-bdorney-status-blocked]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Blocked%22
[search-CMS_GEM_Analysis_Framework-repo-status-wrong-repo]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Wrong+Repo%22
[search-bdorney-status-wrong-repo]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Wrong+Repo%22
[search-CMS_GEM_Analysis_Framework-repo-status-help-wanted]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Help+Wanted%22
[search-bdorney-status-help-wanted]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Help+Wanted%22
[search-CMS_GEM_Analysis_Framework-repo-status-revision-needed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Revision+Needed%22
[search-bdorney-status-revision-needed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Revision+Needed%22
[search-CMS_GEM_Analysis_Framework-repo-status-review-needed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+Review+Needed%22
[search-bdorney-status-review-needed]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+Review+Needed%22
[search-CMS_GEM_Analysis_Framework-repo-status-on-hold]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+On+Hold%22
[search-bdorney-status-on-hold]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+On+Hold%22
[search-CMS_GEM_Analysis_Framework-repo-status-in-progress]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+repo%3ACMS_GEM_Analysis_Framework+user%3Abdorney+label%3A%22Status%3A+In+Progress%22
[search-bdorney-status-in-progress]: https://github.com/issues?utf8=%E2%9C%93&q=is%3Aopen+is%3Aissue+user%3Abdorney+label%3A%22Status%3A+In+Progress%22
