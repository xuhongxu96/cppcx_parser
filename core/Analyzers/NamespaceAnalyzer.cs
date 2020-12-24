using Antlr4.Runtime.Misc;
using cppcx.Core.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class NamespaceAnalyzer : CPPCXParserBaseListener
    {
        public class Namespace : IEquatable<Namespace>
        {
            public string Name { get; set; }
            public CPPCXParser.DeclarationseqContext Context { get; set; }
            public Dictionary<string, Namespace> NestedNamespaces { get; init; } = new Dictionary<string, Namespace>();
            public Namespace Parent { get; set; }

            public override bool Equals(object obj)
            {
                return Equals(obj as Namespace);
            }

            public bool Equals(Namespace other)
            {
                return other != null &&
                       Name == other.Name &&
                       EqualityComparer<Namespace>.Default.Equals(Parent, other.Parent);
            }

            public override int GetHashCode()
            {
                return HashCode.Combine(Name, Parent);
            }

            public static bool operator ==(Namespace left, Namespace right)
            {
                return EqualityComparer<Namespace>.Default.Equals(left, right);
            }

            public static bool operator !=(Namespace left, Namespace right)
            {
                return !(left == right);
            }
        }

        public Namespace Root { get; } = new Namespace { Name = string.Empty };

        private Namespace _current;

        public NamespaceAnalyzer()
        {
            _current = Root;
        }

        public override void EnterNamespaceDefinition([NotNull] CPPCXParser.NamespaceDefinitionContext context)
        {
            var qualifiedNs = context.qualifiednamespacespecifier();
            if (qualifiedNs == null) return;

            var nsParts = new List<string>();

            var nestedNames = qualifiedNs.nestedNameSpecifier();
            if (nestedNames != null)
            {
                nsParts.AddRange(nestedNames.GetText().Split("::", StringSplitOptions.RemoveEmptyEntries));
            }

            nsParts.Add(qualifiedNs.namespaceName().GetText());

            foreach (var part in nsParts)
            {
                if (!_current.NestedNamespaces.TryGetValue(part, out var nestedNs))
                {
                    nestedNs = new Namespace { Name = part, Parent = _current };
                    _current.NestedNamespaces.Add(part, nestedNs);
                }
                _current = nestedNs;
            }

            _current.Context = context.namespaceBody;
        }

        public override void ExitNamespaceDefinition([NotNull] CPPCXParser.NamespaceDefinitionContext context)
        {
            _current = _current.Parent;
        }
    }
}
