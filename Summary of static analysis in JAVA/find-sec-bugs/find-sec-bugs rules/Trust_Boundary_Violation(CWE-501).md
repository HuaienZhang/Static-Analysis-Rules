###Trust Boundary Violation

信任边界违规

**Bug Pattern：**TRUST_BOUNDARY_VIOLATION
####描述：
“信任边界可以被认为是通过一个程序绘制的线，在该线的一边，数据是不可信的，在该线的另一边，数据被认为是可信的，验证逻辑的目的是允许数据 安全地越过信任边界 - 从不可信到可信当一个程序模糊可信任和不可信任之间的界限时，发生信任边界违反。通过在相同数据结构中组合可信和不可信数据， 程序员错误地信任未经验证的数据。“[1]

- [x] 安全相关

####风险代码：
```
public void doSomething(HttpServletRequest req, String activateProperty) {

    //..



    req.getSession().setAttribute(activateProperty,"true");



}

```
```
public void loginEvent(HttpServletRequest req, String userSubmitted) {

    //..



    req.getSession().setAttribute("user",userSubmitted);

}

```


#### Check级别：AST

####解决方案：
The solution would be to add validation prior setting a new session attribute. When possible, prefer data from safe location rather than using direct user input.

####相关信息：
[[1] CWE-501: Trust Boundary Violation]( https://cwe.mitre.org/data/definitions/501.html)
[OWASP : Trust Boundary Violation]( https://www.owasp.org/index.php/Trust_Boundary_Violation)

