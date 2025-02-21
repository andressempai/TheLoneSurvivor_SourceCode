<AnimDB FragDef="animations/mannequin/adb/zombie_fragmentIds.xml" TagDef="animations/mannequin/adb/zombie_tags.xml">
	<FragmentList>
		<idle>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="zombie_idle" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</idle>
		<walk>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.2"/>
					<Animation name="zombie_walk_fwd" flags="Loop"/>
				</AnimLayer>
			</Fragment>
		</walk>
		<melee>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="zombie_attack_fwd"/>
				</AnimLayer>
			</Fragment>
		</melee>
		<hit_reaction_fwd>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="zombie_hit_reaction_fwd"/>
				</AnimLayer>
			</Fragment>
		</hit_reaction_fwd>
		<dead>
			<Fragment BlendOutDuration="0.2" Tags="">
				<AnimLayer>
					<Blend ExitTime="0" StartTime="0" Duration="0.1"/>
					<Animation name="zombie_head_shake"/>
				</AnimLayer>
			</Fragment>
		</dead>
	</FragmentList>
</AnimDB>