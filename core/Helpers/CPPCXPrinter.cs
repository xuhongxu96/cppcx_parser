using Antlr4.Runtime;
using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Text;

namespace cppcx.Core.Helpers
{
    public class CPPCXPrinter : IParseTreeListener
    {
        private int _level = 0;
        private readonly StringBuilder _sb = new StringBuilder();

        public string Content => _sb.ToString();

        public void EnterEveryRule(ParserRuleContext ctx)
        {
            _sb.AppendLine($"{new string(' ', _level * 2)} {ctx.GetType().Name} {ctx.GetText()}");
            ++_level;
        }

        public void ExitEveryRule(ParserRuleContext ctx)
        {
            --_level;
        }

        public void VisitErrorNode(IErrorNode node)
        {
        }

        public void VisitTerminal(ITerminalNode node)
        {
        }
    }
}
