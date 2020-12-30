using Antlr4.Runtime;
using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class TypedNameAnalyzer
    {
        [DebuggerDisplay("{Type} {Name}")]
        public class TypedName
        {
            public string Type { get; set; }
            public string Name { get; set; }
        }

        private void MovePointerToType(ref string type, ref string name, char pointer)
        {
            if (name.StartsWith('*'))
            {
                name = name.TrimStart('*');
                type += '*';
            }
        }

        public TypedName Visit([NotNull] IEnumerable<IParseTree> contexts)
        {
            var typeName = string.Join(' ', contexts.Select(o => { var listener = new Listener(); ParseTreeWalker.Default.Walk(listener, o); return listener.Content; }));
            var i = typeName.LastIndexOf(' ');
            Debug.Assert(i != -1);
            var type = typeName.Substring(0, i);
            var name = typeName.Substring(i + 1);

            MovePointerToType(ref type, ref name, '*');
            MovePointerToType(ref type, ref name, '&');
            MovePointerToType(ref type, ref name, '^');

            return new TypedName
            {
                Type = type,
                Name = name,
            };
        }

        public string VisitFunctionDeclarationReturnType([NotNull] CPPCXParser.SimpleDeclarationContext context)
        {
            var type = string.Join(' ', context.declSpecifierSeq().declSpecifier().Select(o => o.GetText()));
            var nameAndParams = context.initDeclaratorList().initDeclarator()[0].declarator().pointerDeclarator().GetText();
            MovePointerToType(ref type, ref nameAndParams, '*');
            MovePointerToType(ref type, ref nameAndParams, '&');
            MovePointerToType(ref type, ref nameAndParams, '^');

            return type;
        }

        private class Listener : IParseTreeListener
        {
            private StringBuilder _sb = new StringBuilder();

            public string Content => _sb.ToString().TrimEnd();

            public void VisitTerminal(ITerminalNode node)
            {
                var t = node.GetText().Trim();
                if ((t.StartsWith("::")) && _sb.Length > 0)
                    --_sb.Length;

                _sb.Append(t);

                if (!t.EndsWith("::"))
                    _sb.Append(' ');
            }

            public void VisitErrorNode(IErrorNode node)
            {
            }

            public void EnterEveryRule(ParserRuleContext ctx)
            {
            }

            public void ExitEveryRule(ParserRuleContext ctx)
            {
            }
        }
    }
}
