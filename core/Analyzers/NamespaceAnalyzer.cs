using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using cppcx.Core.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class NamespaceAnalyzer : CPPCXParserBaseVisitor<NamespaceAnalyzer.Namespace>
    {
        public class Namespace : IEquatable<Namespace>
        {
            public string Name { get; set; }
            public List<CPPCXParser.DeclarationseqContext> Contexts { get; init; } = new List<CPPCXParser.DeclarationseqContext>();
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

        public override Namespace VisitNamespaceDefinition([NotNull] CPPCXParser.NamespaceDefinitionContext context)
        {
            var childNs = VisitChildren(context);

            var qualifiedNs = context.qualifiednamespacespecifier();
            if (qualifiedNs == null) return childNs;

            var nsParts = new List<string>();

            var nestedNames = qualifiedNs.nestedNameSpecifier();
            if (nestedNames != null)
            {
                nsParts.AddRange(nestedNames.GetText().Split("::", StringSplitOptions.RemoveEmptyEntries));
            }

            nsParts.Add(qualifiedNs.namespaceName().GetText());

            var res = new Namespace { Name = string.Empty };

            Namespace current = res;
            foreach (var part in nsParts)
            {
                if (!res.NestedNamespaces.TryGetValue(part, out var nestedNs))
                {
                    nestedNs = new Namespace { Name = part, Parent = current };
                    current.NestedNamespaces.Add(part, nestedNs);
                }
                current = nestedNs;
            }

            current.Contexts.Add(context.namespaceBody);

            if (childNs != null)
            {
                foreach (var ns in childNs.NestedNamespaces)
                {
                    ns.Value.Parent = current;
                    current.NestedNamespaces.Add(ns.Key, ns.Value);
                }
            }

            return res;
        }

        protected override Namespace AggregateResult(Namespace aggregate, Namespace nextResult)
        {
            if (nextResult == null) return aggregate;
            if (aggregate == null) return nextResult;

            Debug.Assert(aggregate.Name == nextResult.Name);

            var intersectNs = aggregate.NestedNamespaces.Keys.Intersect(nextResult.NestedNamespaces.Keys);
            foreach (var sameNsName in intersectNs)
            {
                AggregateResult(aggregate.NestedNamespaces[sameNsName], nextResult.NestedNamespaces[sameNsName]);
            }
            foreach (var nsName in nextResult.NestedNamespaces.Keys.Except(intersectNs))
            {
                var ns = nextResult.NestedNamespaces[nsName];
                aggregate.NestedNamespaces.Add(nsName, ns);
            }

            aggregate.Contexts.AddRange(nextResult.Contexts);
            return aggregate;
        }
    }
}
